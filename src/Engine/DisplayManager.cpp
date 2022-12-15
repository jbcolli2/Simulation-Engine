//
// Created by jcollin2 on 7/1/22.
//

#include <iostream>

#include "DisplayManager.h"


/***************** DisplayManager::StartUp  ******************
     * @brief Initialize the window and context.
     *
     * @param width, height Dimensions of the window
******************************************************************///
int seng::DisplayManager::StartUp(int width, int height)
{
    m_aspectRatio = static_cast<float>(width)/height;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
//    glfwWindowHint(GLFW_SAMPLES, 4);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    m_window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    if (m_window == NULL)
    {
        std::cout << "Failed to create glfw window\n";
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(m_window);

    glfwSetKeyCallback(m_window, ::seng::keyCallback);
    glfwSetCursorPosCallback(m_window, ::seng::cursorPositionCallback);
    glfwSetMouseButtonCallback(m_window, ::seng::mouseButtonCallback);



    return 1;
}




/***************** DisplayManager::ShutDown  ******************
     * @brief Terminate the window.  Clean up any window related resources that were created.
******************************************************************///
void seng::DisplayManager::ShutDown()
{
    glfwTerminate();
    m_window = nullptr;
}




void seng::DisplayManager::GetScreenDimensions(int* width, int* height)
{
    glfwGetFramebufferSize(m_window, width, height);
}
