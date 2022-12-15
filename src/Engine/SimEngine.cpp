//
// Created by jcollin2 on 6/30/22.
//

#include "glad/glad.h"

#include "SimEngine.h"

using namespace seng;



int SimEngine::StartUp(int width, int height)
{

    if(!m_displayManager.StartUp(width, height))
    {
        std::cout << "Failed to startup display manager\n";
        return 0;
    }
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 0;
    }

    m_sceneManager = new PrimScene1();
    m_sceneManager->StartUp(&m_displayManager);

    int screenWidth, screenHeight;
    m_displayManager.GetScreenDimensions(&screenWidth, &screenHeight);
    if(!m_renderer.StartUp(m_sceneManager, screenWidth, screenHeight))
    {
        std::cout << "Failed to startup renderer\n";
        return 0;
    }

    return 1;
}


/***************** SimEngine::Shutdown  ******************
* @brief Clean up all the elements of the engine after being done running the simulation.
******************************************************************///
void SimEngine::ShutDown()
{
    m_displayManager.ShutDown();
    m_sceneManager->ShutDown();
    m_renderer.ShutDown();

    delete m_sceneManager;
}




/***************** SimEngine::Run  ******************
     * @brief Run the game loop.  Break out of the loop when the window closes by
     *      the glfw
******************************************************************///
void SimEngine::Run()
{
    float currentFrame = 0.f;
    float lastFrame = 0.f;
    float deltaTime = 0.f;
    float physDeltaTime = 0.f;


    while(!glfwWindowShouldClose(m_displayManager.m_window))
    {
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        Input::GetInstance().PollInput();

        if(Input::GetInstance().KeyDown(GLFW_KEY_P))
        {
            glfwSetWindowShouldClose(m_displayManager.m_window, true);
        }


        m_sceneManager->Update(deltaTime, deltaTime);

        m_renderer.Render();

        glfwSwapBuffers(m_displayManager.m_window);
    }
}