//
// Created by jcollin2 on 7/3/22.
//

#ifndef SIM_ENGINE_RENDERER_H
#define SIM_ENGINE_RENDERER_H

#include "Misc/Common.h"
#include "Rendering/Shader.h"



namespace seng
{

//***********************************************************
//       Texture Quad
//***********************************************************
class TextureQuad
{
private:
    unsigned int m_vao{0};
    unsigned int m_vbo{0};

    std::vector<Vert3x2f> m_vertices;

public:
    TextureQuad();
    ~TextureQuad();
    TextureQuad(const TextureQuad& other) = delete;
    TextureQuad& operator=(const TextureQuad& other) = delete;
    TextureQuad(TextureQuad&& other);
    TextureQuad& operator=(TextureQuad&& other);

    void StartUp();
    void Render(unsigned int tbo);
};



//***********************************************************
//       Framebuffers
//***********************************************************
class FrameBufferFP
{
private:
    unsigned int m_fbo;             // Handle for framebuffer object
    unsigned int m_rbo;             // Handle for renderbuffer object

    unsigned int m_tbo;             // handle to floating point texture

    int m_screenWidth;
    int m_screenHeight;
public:
    /***************** Ctor  ******************
     * @brief Setup a floating point frame buffer.
    ******************************************************************///
    FrameBufferFP() = default;
    FrameBufferFP(int screenWidth, int screenHeight);
    FrameBufferFP(const FrameBufferFP&) = delete;
    FrameBufferFP& operator=(const FrameBufferFP&) = delete;
    FrameBufferFP(FrameBufferFP&&);
    FrameBufferFP& operator=(FrameBufferFP&&);
    ~FrameBufferFP();


    void Bind();
    void Release();
    unsigned int GetTBO();
};





//***********************************************************
//       Renderer
//***********************************************************
class SceneManager;
class Renderer
{
private:
    unsigned int m_uboVP{0};
    int m_screenWidth;
    int m_screenHeight;

    ///////////////// Shader Programs ///////////////////////////////////////
    Shader m_solidShader{};
    Shader m_phongShader{};
    Shader m_passShader{};
    Shader m_hdrShader{};
    Shader* m_currentShader{};

    /////////////  Managers  ///////////////////
    SceneManager* m_sceneManager{nullptr};

    /////////////  Framebuffers  ///////////////////
    FrameBufferFP m_hdrFB;
    TextureQuad m_texQuad;


    /////////////  Utility Methods  ///////////////////
    void DrawScene();
public:
    Renderer() = default;

    int StartUp(SceneManager* sceneManager, int screenWidth, int screenHeight);
    void ShutDown(){};

    void Render();
};

} // End namespace seng

#endif //SIM_ENGINE_RENDERER_H
