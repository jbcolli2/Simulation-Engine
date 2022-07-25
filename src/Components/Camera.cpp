//
// Created by jcollin2 on 7/19/22.
//


#include "Camera.h"

namespace seng
{

const std::string Camera::positionUniformName{"cameraPosition"};


Camera::Camera()
{
    setView();
    m_proj = glm::perspective(m_fov, DisplayManager::m_aspectRatio, m_nearField, m_farField);
}

Camera::Camera(const glm::vec3& position, const glm::vec3& direction) :
    m_position(position), m_direction(direction)
{
    setView();
    m_proj = glm::perspective(m_fov, DisplayManager::m_aspectRatio, m_nearField, m_farField);
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




} // end namespace seng

