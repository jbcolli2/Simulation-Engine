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

    Shader& m_shader;

public:
    ScreenTexture(Shader& shader) : m_shader(shader) {};
    ScreenTexture(const ScreenTexture& other) = delete;
    ScreenTexture& operator=(const ScreenTexture& other) = delete;
    ScreenTexture(ScreenTexture&& other) = delete;
    ScreenTexture& operator=(ScreenTexture&& other) = delete;

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





class SceneManager;
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


    /////////////  Shaders  ///////////////////
    std::unordered_map<ShaderType, std::unique_ptr<Shader>> m_shaderTable{};

    void SetMaterialShader(Material* material);

public:
    Renderer() = default;

    int StartUp(SceneManager* sceneManager);
    void ShutDown(){};

    void Render();
    void DrawScene();


};

} // End namespace seng

#endif //SIM_ENGINE_RENDERER_H
