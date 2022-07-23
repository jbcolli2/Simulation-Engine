//
// Created by jcollin2 on 7/3/22.
//

#ifndef SIM_ENGINE_RENDERER_H
#define SIM_ENGINE_RENDERER_H

#include "Misc/Util.h"
#include "Scene/SceneManager.h"
#include "Engine/DisplayManager.h"
#include "Rendering/Shader.hpp"



namespace seng
{

class Renderer
{
private:
    float r{.8f}, g{.4f}, b{.4f};

    unsigned int m_uboVP{0};

    // Shader programs
    Shader m_solidShader{};

    // Managers
    SceneManager* m_sceneManager;
public:
    Renderer(){};

    int StartUp(SceneManager* sceneManager);
    void ShutDown(){};

    void Render();
};

} // End namespace seng

#endif //SIM_ENGINE_RENDERER_H
