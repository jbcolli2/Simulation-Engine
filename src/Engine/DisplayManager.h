//
// Created by jcollin2 on 7/1/22.
//
#include <glad/glad.h>
#include "GLFW/glfw3.h"

#include "Input.h"




#ifndef SIM_ENGINE_DISPLAYMANAGER_H
#define SIM_ENGINE_DISPLAYMANAGER_H


namespace seng
{

void framebuffer_resize_callback(GLFWwindow* window, int width, int height);


struct DisplayManager
{
public:
    // Window dimensions
    float m_aspectRatio{4.f/3.f};

    GLFWwindow* m_window;

    /***************** StartUp  ******************
         * @brief Initialize the window and glfw window hints.
         *
         * @param width, height Dimensions of the window.
    ******************************************************************///
    int StartUp(int width, int height);

    /***************** ShutDown  ******************
         * @brief Terminate the window.  Clean up any window related resources that were created.
    ******************************************************************///
    void ShutDown();
}; // End DisplayManager definition



} // End namespace seng



#endif //SIM_ENGINE_DISPLAYMANAGER_H
