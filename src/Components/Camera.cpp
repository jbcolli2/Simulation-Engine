//
// Created by jcollin2 on 7/19/22.
//


#include "Camera.h"

namespace seng
{

const std::string Camera::positionUniformName{"cameraPosition"};


Camera::Camera(DisplayManager* displayManager)
{
    setView();
    m_proj = glm::perspective(m_fov, displayManager->m_aspectRatio, m_nearField, m_farField);
}

Camera::Camera(const glm::vec3& position, const glm::vec3& direction, DisplayManager* displayManager) :
    m_position(position), m_direction(direction)
{
    setView();
    m_proj = glm::perspective(m_fov, displayManager->m_aspectRatio, m_nearField, m_farField);
}










void Camera::IncrementDirection(float yawInc, float pitchInc)
{
    m_yaw += yawInc;
    m_pitch += pitchInc;

    SetDirection(m_yaw, m_pitch);
}


/***************** SetUniforms  ******************
 * @brief Set the uniforms on a shader with camera information.
 *      Right now this is just camera position for specular reflection.
 *
 * @param shader Shader being used that has uniforms.
******************************************************************///
void Camera::SetUniforms(Shader& shader)
{
    shader.setUniform3f(positionUniformName, m_position.x, m_position.y, m_position.z);
}

//***********************************************************
//       Setters
//***********************************************************
void Camera::SetDirection(float yaw, float pitch)
{
    m_yaw = yaw;
    m_pitch = pitch;
    m_direction = angleToDirection(yaw, pitch);
    computeCamRight();
    setView();
}



//***********************************************************
//       Private methods
//***********************************************************

void Camera::setView()
{
    computeCamRight();
    m_view = glm::lookAt(m_position, m_position + m_direction, m_camUp);
}

/***************** angleToDirection  ******************
 * @brief Convert pitch and yaw into cartesian direction.
 *
 * @param yaw horizontal angle of direction in degrees [-90, 90]
 * @param pitch vertical angle of direction in degrees.  [-90, 90]
******************************************************************///
glm::vec3 Camera::angleToDirection(float yaw, float pitch)
{
    // Don't allow camera to have angle outside of [-90, 90]
    if(pitch > 89) pitch = 89;
    if(pitch < -89) pitch = -89;

    float theta = glm::radians(pitch);
    float phi = glm::radians(yaw);

    glm::vec3 direction{};
    direction.x = glm::cos(theta)*glm::sin(phi);
    direction.y = glm::sin(theta);
    direction.z = -glm::cos(theta)*glm::cos(phi);

    return direction;
}


/***************** computeCamRight  ******************
 * @brief Compute normalized right vector for camera and assign it to m_camRight. 
******************************************************************///
void Camera::computeCamRight()
{
    m_camRight = glm::normalize(glm::cross(m_direction, m_camUp));
}
















//***********************************************************
//       CameraController
//***********************************************************

void CameraController::StartUp()
{
    glfwSetInputMode(m_dispManager.m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void CameraController::Update(float deltaTime)
{
    Input* input = &Input::GetInstance();

    ///////////////// Rotate Camera ///////////////////////////////////////
    if(m_oldMousePosition.x < 0.f)
        m_oldMousePosition = input->MousePosition();

    glm::vec2 newMousePosition = input->MousePosition();

    glm::vec2 rotateDelta = (newMousePosition - m_oldMousePosition)*deltaTime*m_rotateSpeed;
    rotateDelta.y = -rotateDelta.y;
    m_oldMousePosition = newMousePosition;

    m_camera.IncrementDirection(rotateDelta.x, rotateDelta.y);



    ///////////////// Pan Camera ///////////////////////////////////////
    float positionDelta = m_panSpeed*deltaTime;
    if(input->KeyPress(GLFW_KEY_W))
    {
        m_camera.m_position += positionDelta*m_camera.GetDirection();
    }
    if(input->KeyPress(GLFW_KEY_S))
    {
        m_camera.m_position -= positionDelta*m_camera.GetDirection();
    }
    if(input->KeyPress(GLFW_KEY_A))
    {
        m_camera.m_position -= positionDelta*m_camera.GetRight();
    }
    if(input->KeyPress(GLFW_KEY_D))
    {
        m_camera.m_position += positionDelta*m_camera.GetRight();
    }
    if(input->KeyPress(GLFW_KEY_Q))
    {
        m_camera.m_position -= positionDelta*m_camera.GetUp();
    }
    if(input->KeyPress(GLFW_KEY_E))
    {
        m_camera.m_position += positionDelta*m_camera.GetUp();
    }



    ///////////////// Zero Camera ///////////////////////////////////////
    if(input->KeyDown(GLFW_KEY_0))
    {
        m_camera.SetDirection(0.f, 0.f);
        m_camera.m_position = glm::vec3(0.f);
    }

}



} // end namespace seng

