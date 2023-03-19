//
// Created by jcollin2 on 7/3/22.
//

#include "Renderer.h"
#include "Rendering/Mesh.h"
#include "Rendering/Material.h"

#include "Components/Camera.h"
#include "Components/Lights.h"

#include "Engine/Object.h"

#include "Scene/SceneManager.h"

using namespace seng;





int Renderer::StartUp(SceneManager* sceneManager)
{
    m_sceneManager = sceneManager;

    // Setup all shaders
    auto materialList = m_sceneManager->m_scene.GetAllMaterials();
    for(auto material : materialList)
    {
        ShaderType shaderType = material->m_shaderType;
        try
        {
            Shader* shaderPtr = m_shaderTable.at(shaderType).get();
            material->m_shader = shaderPtr;
        }
        catch(std::out_of_range e)
        {
            m_shaderTable[shaderType] = std::make_unique<Shader>(Shader::m_vertexShaderFile[shaderType],
                                                                 Shader::m_fragShaderFile[shaderType]);
            material->m_shader = m_shaderTable[shaderType].get();
        }
    }

    m_currentShader = nullptr;

    ///////////////// Uniform buffers for view/proj matrices ///////////////////////////////////////
    glGenBuffers(1, &m_uboVP);
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboVP);
    glBufferData(GL_UNIFORM_BUFFER, 16*8, nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_uboVP);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    return 1;
}






void Renderer::Render()
{
    Scene& scene = m_sceneManager->m_scene;
    ///////////////// Clear color buffer ///////////////////////////////////////
    glEnable(GL_DEPTH_TEST);
    glClearColor(.1, .1, .1, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Camera& mainCamera = scene.m_objects[scene.m_cameras[0]]->GetComponent<Camera>();

    // Fill UBO with view and proj matrices
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboVP);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(mainCamera.m_view));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(mainCamera.m_proj));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    m_currentShader = m_shaderTable[ShaderType::VPASS_FLIT].get();
    m_currentShader->startProgram();
    /////////////  Set Camera and Lights in Shader  ///////////////////
    m_currentShader->setUniform1f("ambientIntensity", scene.m_ambientIntensity);
    m_currentShader->setUniform3f("cameraPosition", mainCamera.m_position.x, mainCamera.m_position.y, mainCamera.m_position.z);
    for(unsigned int lightIndex : scene.m_lights)
    {
        Object* light = scene.m_objects[lightIndex].get();
        if(light->HasComponent<PointLight>())
        {
            light->GetComponent<PointLight>().SetUniforms(*m_currentShader);
        }
        if(light->HasComponent<DirLight>())
        {
            light->GetComponent<DirLight>().SetUniforms(*m_currentShader);
        }
    }

    /////////////  Draw all objects with a Mesh  ///////////////////
    for(auto& obj : scene.m_objects)
    {
        if(obj->HasComponent<Mesh>())
        {
            Mesh& mesh = obj->GetComponent<Mesh>();

            Shader* nextShader = mesh.GetShader();
            if(nextShader != m_currentShader)
            {
                m_currentShader = nextShader;
                m_currentShader->startProgram();
            }

            mesh.UpdateModelMatrix();
            m_currentShader->setUniformMatrix4f("model", mesh.m_model);
            mesh.Render();
         }
    }
    m_currentShader->stopProgram();

}


