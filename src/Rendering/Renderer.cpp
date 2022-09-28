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

    m_solidShader = Shader{"../assets/shaders/PosNormUVVert.glsl", "../assets/shaders/SolidColorFrag.glsl"};
    m_phongShader = Shader{"../assets/shaders/PosNormUVVert.glsl", "../assets/shaders/LightFrag.glsl"};
    m_currentShader = &m_phongShader;

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

    Camera& mainCamera = scene.m_cameras[0]->GetComponent<Camera>();

    // Fill UBO with view and proj matrices
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboVP);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(mainCamera.m_view));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(mainCamera.m_proj));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    m_currentShader->startProgram();
    /////////////  Set Camera and Lights in Shader  ///////////////////
    m_currentShader->setUniform1f("ambientIntensity", scene.m_ambientIntensity);
    m_currentShader->setUniform3f("cameraPosition", mainCamera.m_position.x, mainCamera.m_position.y, mainCamera.m_position.z);
    for(Object* light : scene.m_lights)
    {
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
            mesh.UpdateModelMatrix();
            m_currentShader->setUniformMatrix4f("model", mesh.m_model);
            for(MeshData* meshdata : mesh.m_meshes)
            {
                meshdata->Render(*m_currentShader);
            }
        }
    }
    m_currentShader->stopProgram();

}


