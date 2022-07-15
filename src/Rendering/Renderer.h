//
// Created by jcollin2 on 7/3/22.
//

#ifndef SIM_ENGINE_RENDERER_H
#define SIM_ENGINE_RENDERER_H

#include "Misc/Util.h"
#include "Scene/SceneManager.h"
#include "Rendering/Shader.hpp"



namespace seng
{

class Renderer
{
private:
    float r{.8f}, g{.4f}, b{.4f};
public:
    Renderer(){};

    int StartUp(const SceneManager& sceneManager);
    void ShutDown(){};

    void Render();
};

} // End namespace seng

#endif //SIM_ENGINE_RENDERER_H
