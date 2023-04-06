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
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void ScreenTexture::StartUp()
{
    m_vertices = std::vector<Vert3x3x2f>{
            Vert3x3x2f(-1.f, 1.0f, 0.0f, 0.f, 0.f, 0.f, 0.f, 1.f),
            Vert3x3x2f(-1.f, -3.f, 0.0f, 0.f, 0.f, 0.f, 0.f, -1.f),
            Vert3x3x2f(3.f, 1.f, 0.0f, 0.f, 0.f, 0.f, 2.f, 1.f)
    };

    m_vao = VAO(m_vertices);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------------------------------------------------------------------------------------





//***********************************************************
//       Float16 Attachment Code
//***********************************************************
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
FBAttachmentF16::~FBAttachmentF16()
{
    glDeleteTextures(1, &m_id);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void FBAttachmentF16::Setup(unsigned int bufferWidth, unsigned int bufferHeight)
{
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, bufferWidth, bufferHeight, 0, GL_RGBA, GL_FLOAT, NULL);

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void FBAttachmentF16::Attach()
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_id);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------



//***********************************************************
//       Depth-only Renderbuffer Attachment Code
//***********************************************************
//------------------------------------------------------------------------------------------------------------------------------------------------
FBAttachmentRenderDepth::~FBAttachmentRenderDepth()
{
    glDeleteRenderbuffers(1, &m_id);
}
//------------------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------------------
void FBAttachmentRenderDepth::Setup(unsigned int bufferWidth, unsigned int bufferHeight)
{
    glGenRenderbuffers(1, &m_id);
    glBindRenderbuffer(GL_RENDERBUFFER, m_id);
    glRenderbufferStorage(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT, bufferWidth, bufferHeight);
}
//------------------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------------------
void FBAttachmentRenderDepth::Attach()
{
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_id);
}


//***********************************************************
//       Framebuffer Code
//***********************************************************
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
FrameBuffer::FrameBuffer(Attachment* primary, Attachment* secondary) :
    m_primaryAttachment(primary), m_secondaryAttachment(secondary)
{

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------------------------------------------------------------
FrameBuffer::FrameBuffer(FrameBuffer&& other) :
        m_fbo(other.m_fbo), m_bufferWidth(other.m_bufferWidth),
        m_bufferHeight(other.m_bufferHeight)
{
    other.m_fbo = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other)
{
    if(this == &other)
        return *this;

    m_fbo = other.m_fbo;
    m_bufferWidth = other.m_bufferWidth;
    m_bufferHeight = other.m_bufferHeight;

    other.m_fbo = 0;

    return *this;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------------------------
FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &m_fbo);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void FrameBuffer::StartUp(unsigned int bufferWidth, unsigned int bufferHeight)
{
    m_bufferHeight = bufferHeight;
    m_bufferWidth = bufferWidth;

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    m_primaryAttachment->Setup(m_bufferWidth, m_bufferHeight);
    m_primaryAttachment->Attach();
    if(m_secondaryAttachment != nullptr)
    {
        m_secondaryAttachment->Setup(m_bufferWidth, m_bufferHeight);
        m_secondaryAttachment->Attach();
    }


    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void FrameBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
void FrameBuffer::Release()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------







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
