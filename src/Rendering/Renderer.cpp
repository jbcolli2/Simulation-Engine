//
// Created by jcollin2 on 7/3/22.
//

#include "Renderer.h"
#include "Rendering/Mesh.h"
#include "Rendering/Assets.h"

#include "Components/Camera.h"
#include "Components/Lights.h"

#include "Engine/Object.h"

#include "Scene/SceneManager.h"

using namespace seng;

//***********************************************************
//       Texture Quad
//***********************************************************


void ScreenTexture::StartUp()
{
    m_vertices = std::vector<Vert3x3x2f>{
            Vert3x3x2f(-1.f, 1.0f, 0.0f, 0.f, 0.f, 0.f, 0.f, 1.f),
            Vert3x3x2f(-1.f, -3.f, 0.0f, 0.f, 0.f, 0.f, 0.f, -1.f),
            Vert3x3x2f(3.f, 1.f, 0.0f, 0.f, 0.f, 0.f, 2.f, 1.f)
    };

    m_vao = VAO(m_vertices);
}




void ScreenTexture::Render(unsigned int tbo)
{
    m_shader.startProgram();
    glBindVertexArray(m_vao.m_vaoID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tbo);
    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
    glBindVertexArray(0);
    m_shader.stopProgram(); 
}


//***********************************************************
//       Framebuffer Code
//***********************************************************
FrameBufferFP::FrameBufferFP(int screenWidth, int screenHeight) : m_screenWidth(screenWidth),
                                                                  m_screenHeight(screenHeight)
{
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    /////////////  Bind texture to FBO  ///////////////////
    glGenTextures(1, &m_tbo);
    glBindTexture(GL_TEXTURE_2D, m_tbo);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_screenWidth, m_screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tbo, 0);

    glGenRenderbuffers(1, &m_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_screenWidth, m_screenHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

    /////////////  Check if complete  ///////////////////
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer NOT complete\n";

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBufferFP::FrameBufferFP(FrameBufferFP&& other) :
        m_fbo(other.m_fbo), m_rbo(other.m_rbo), m_tbo(other.m_tbo), m_screenWidth(other.m_screenWidth),
        m_screenHeight(other.m_screenHeight)
{
    other.m_fbo = 0;
    other.m_rbo = 0;
    other.m_tbo = 0;
}

FrameBufferFP& FrameBufferFP::operator=(FrameBufferFP&& other)
{
    m_fbo = other.m_fbo;
    m_rbo = other.m_rbo;
    m_tbo = other.m_tbo;
    m_screenWidth = other.m_screenWidth;
    m_screenHeight = other.m_screenHeight;

    other.m_fbo = 0;
    other.m_rbo = 0;
    other.m_tbo = 0;

    return *this;
}

FrameBufferFP::~FrameBufferFP()
{
    glDeleteFramebuffers(1, &m_fbo);
    glDeleteRenderbuffers(1, &m_rbo);
    glDeleteTextures(1, &m_tbo);
}


void FrameBufferFP::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void FrameBufferFP::Release()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int FrameBufferFP::GetTBO()
{
    return m_tbo;
}




int Renderer::StartUp(SceneManager* sceneManager)
{
    m_sceneManager = sceneManager;

    // Setup all shaders
    auto materialList = m_sceneManager->m_scene.GetAllMaterials();
    for(auto material : materialList)
    {
        SetMaterialShader(material);
    }

    auto modelList = m_sceneManager->m_scene.GetAllModels();
    for(auto model : modelList)
    {
        auto modelMaterialList = model->GetAllMaterials();
        for(auto material : modelMaterialList)
        {
            SetMaterialShader(material);
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

void Renderer::SetMaterialShader(Material* material)
{
    ShaderType shaderType = material->m_shaderType;
    try
    {
        Shader* shaderPtr = m_shaderTable.at(shaderType).get();
        material->m_shader = shaderPtr;
    }
    catch(std::out_of_range e)
    {
        m_shaderTable[shaderType] = std::__1::make_unique<Shader>(Shader::m_vertexShaderFile[shaderType],
                                                                  Shader::m_fragShaderFile[shaderType]);
        material->m_shader = m_shaderTable[shaderType].get();
    }
}


void Renderer::Render()
{
    ///////////////// Clear color buffer ///////////////////////////////////////
    glEnable(GL_DEPTH_TEST);
    glClearColor(.1, .1, .1, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    DrawScene();
}


void Renderer::DrawScene()
{
    Scene& scene = m_sceneManager->m_scene;

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
