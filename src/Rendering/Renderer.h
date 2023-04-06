//
// Created by jcollin2 on 7/3/22.
//

#ifndef SIM_ENGINE_RENDERER_H
#define SIM_ENGINE_RENDERER_H

#include "Misc/Common.h"
#include "Misc/MeshCommon.h"
#include "Rendering/Shader.h"
#include "Assets.h"


namespace seng
{

//***********************************************************
//       Texture Quad
//***********************************************************
class ScreenTexture
{
private:
    VAO m_vao;                          // VAO object holding vertex buffer info

    std::vector<Vert3x3x2f> m_vertices; // hold vertex info of screen triangle

    Shader m_shader;

public:
    ScreenTexture(const std::string& vertFilename, const std::string& fragFilename) : m_shader(vertFilename, fragFilename) {};
    ScreenTexture(const ScreenTexture& other) = delete;
    ScreenTexture& operator=(const ScreenTexture& other) = delete;
    ScreenTexture(ScreenTexture&& other) = delete;
    ScreenTexture& operator=(ScreenTexture&& other) = delete;

    void StartUp();
    void Render(unsigned int tbo);
};





//***********************************************************
//       Attachment Class
//***********************************************************
class Attachment
{
public:
    unsigned int m_id{0};

    virtual ~Attachment() = default;

    virtual void Setup(unsigned int bufferWidth, unsigned int bufferHeight) = 0;
    virtual void Attach() = 0;
};


//----------------------------------------------------------------------------------------------------------------------------------------------------------------
class FBAttachmentF16 : public Attachment
{
public:
    ~FBAttachmentF16() override;

    void Setup(unsigned int bufferWidth, unsigned int bufferHeight) override;
    void Attach() override;
};
//------------------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------------------
class FBAttachmentRenderDepth : public Attachment
{
public:
    ~FBAttachmentRenderDepth() override;

    void Setup(unsigned int bufferWidth, unsigned int bufferHeight) override;
    void Attach() override;
};



//----------------------------------------------------------------------------------------------------------------------------------------------------------------
//***********************************************************
//       Framebuffer Class
//***********************************************************
class FrameBuffer
{
private:
    unsigned int m_fbo{0};             // Handle for framebuffer object

    int m_bufferWidth{0};              // Dimensions of buffer
    int m_bufferHeight{0};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
    std::unique_ptr<Attachment> m_primaryAttachment;
    std::unique_ptr<Attachment> m_secondaryAttachment;
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------
public:
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------
    /***************** Framebuffer Ctor  ******************
     * @brief Define the framebuffer by the types of attachements it has
    ******************************************************************///
    FrameBuffer(Attachment* primary, Attachment* secondary = nullptr);
    FrameBuffer(const FrameBuffer&) = delete;
    FrameBuffer& operator=(const FrameBuffer&) = delete;
    FrameBuffer(FrameBuffer&&);
    FrameBuffer& operator=(FrameBuffer&&);
    ~FrameBuffer();
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------


    void StartUp(unsigned int bufferWidth, unsigned int bufferHeight);

    void Bind();
    void Release();

    unsigned int GetPrimaryID() {return m_primaryAttachment->m_id;};
    unsigned int GetSecondaryID() {return m_secondaryAttachment->m_id;};

};






//***********************************************************
//       Renderer Class
//***********************************************************
class SceneManager;
class DisplayManager;
class Renderer
{
private:
    unsigned int m_uboVP{0};

    ///////////////// Shader Programs ///////////////////////////////////////
    Shader m_solidShader{};
    Shader m_phongShader{};
    Shader* m_currentShader{};

    // Managers
    SceneManager* m_sceneManager{nullptr};
    DisplayManager* m_displayManager{nullptr};


    /////////////  Shaders  ///////////////////
    std::unordered_map<ShaderType, std::unique_ptr<Shader>> m_shaderTable{};

    void SetMaterialShader(Material* material);

    FrameBuffer hdrFB{new FBAttachmentF16(), new FBAttachmentRenderDepth};
    ScreenTexture m_screenTex{"../assets/shaders/Vert_ScreenPass.glsl", "../assets/shaders/Frag_HDR.glsl"};

public:
    Renderer() = default;

    int StartUp(DisplayManager* displayManager, SceneManager* sceneManager);
    void ShutDown(){};

    void Render();
    void DrawScene();


};

} // End namespace seng

#endif //SIM_ENGINE_RENDERER_H
