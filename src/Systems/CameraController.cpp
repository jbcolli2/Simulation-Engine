//
// Created by jcollin2 on 7/22/22.
//

#include "CameraController.h"

namespace seng
{

void CameraController::StartUp()
{
    glfwSetInputMode(m_displayManager->m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    m_oldMousePosition = Input::GetInstance().MousePosition();

    m_rotateSpeed = 1.4f;
    m_panSpeed = 1.f;

    m_camera = m_objects[0]->GetComponent<Camera>();
}




void CameraController::Update(float deltaTime)
{
    Input* input = &Input::GetInstance();

    ///////////////// Rotate Camera ///////////////////////////////////////
    glm::vec2 newMousePosition = input->MousePosition();

    glm::vec2 rotateDelta = (newMousePosition - m_oldMousePosition)*deltaTime*m_rotateSpeed;
    rotateDelta.y = -rotateDelta.y;
    m_oldMousePosition = newMousePosition;

    m_camera->IncrementDirection(rotateDelta.x, rotateDelta.y);



    ///////////////// Pan Camera ///////////////////////////////////////
    float positionDelta = m_panSpeed*deltaTime;
    if(input->KeyPress(GLFW_KEY_W))
    {
        m_camera->m_position += positionDelta*m_camera->GetDirection();
    }
    if(input->KeyPress(GLFW_KEY_S))
    {
        m_camera->m_position -= positionDelta*m_camera->GetDirection();
    }
    if(input->KeyPress(GLFW_KEY_A))
    {
        m_camera->m_position -= positionDelta*m_camera->GetRight();
    }
    if(input->KeyPress(GLFW_KEY_D))
    {
        m_camera->m_position += positionDelta*m_camera->GetRight();
    }
    if(input->KeyPress(GLFW_KEY_Q))
    {
        m_camera->m_position -= positionDelta*m_camera->GetUp();
    }
    if(input->KeyPress(GLFW_KEY_E))
    {
        m_camera->m_position += positionDelta*m_camera->GetUp();
    }



    ///////////////// Zero Camera ///////////////////////////////////////
    if(input->KeyDown(GLFW_KEY_0))
    {
        m_camera->SetDirection(0.f, 0.f);
    }
}


} // seng