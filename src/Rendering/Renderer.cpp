//
// Created by jcollin2 on 7/3/22.
//

#include "Renderer.h"

using namespace seng;





int Renderer::StartUp(SceneManager* sceneManager)
{
    m_sceneManager = sceneManager;

    m_solidShader = Shader{"../assets/shaders/PosNormUVVert.glsl", "../assets/shaders/SolidColorFrag.glsl"};
    m_phongShader = Shader{"../assets/shaders/PosNormUVVert.glsl", "../assets/shaders/LightFrag.glsl"};
    m_currentShader = &m_phongShader;

    ///////////////// Uniform buffers for view/proj matrices ///////////////////////////////////////
    glGenBuffers(1, &m_uboVP);
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboVP);
    glBufferData(GL_UNIFORM_BUFFER, 16*8, nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_uboVP);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    return 1;
}






void Renderer::Render()
{
    ///////////////// Clear color buffer ///////////////////////////////////////
    glEnable(GL_DEPTH_TEST);
    glClearColor(.1, .1, .1, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Camera* mainCamera = m_sceneManager->m_scene.GetMainCamera();

    // Fill UBO with view and proj matrices
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboVP);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(mainCamera->m_view));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(mainCamera->m_proj));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    m_currentShader->startProgram();
    m_sceneManager->m_scene.DrawScene(*m_currentShader);
    m_currentShader->stopProgram();

}


