//
// Created by jcollin2 on 6/30/22.
//
#ifndef SIM_ENGINE_SIMENGINE_H
#define SIM_ENGINE_SIMENGINE_H



#include <iostream>

#include "Misc/Common.h"
#include "GLFW/glfw3.h"

#include "DisplayManager.h"
#include "Scene/SceneManager.h"
#include "Engine/Input.h"
#include "Rendering/Renderer.h"


namespace seng
{

class SimEngine
{
private:
    DisplayManager m_displayManager{};
    SceneManager m_sceneManager{};
    Renderer m_renderer{};

public:
    SimEngine() {};

    int StartUp(int width, int height);
    void Run();
    void ShutDown();

};


}




#endif //SIM_ENGINE_SIMENGINE_H
