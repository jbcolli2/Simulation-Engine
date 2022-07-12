//
// Created by jcollin2 on 6/30/22.
//
#ifndef SIM_ENGINE_SIMENGINE_H
#define SIM_ENGINE_SIMENGINE_H



#include <iostream>

#include <GLFW/glfw3.h>

#include "DisplayManager.h"
#include "SceneManager.h"
#include "Input.h"
#include "Renderer.h"


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
