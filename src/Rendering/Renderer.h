//
// Created by jcollin2 on 7/3/22.
//

#ifndef SIM_ENGINE_RENDERER_H
#define SIM_ENGINE_RENDERER_H

#include "Misc/Common.h"
#include "Rendering/Shader.h"



namespace seng
{

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

public:
    Renderer() = default;

    int StartUp(SceneManager* sceneManager);
    void ShutDown(){};

    void Render();
};

} // End namespace seng

#endif //SIM_ENGINE_RENDERER_H
