//
// Created by jcollin2 on 7/19/22.
//


#include "Scene/Camera.h"

namespace seng
{


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

    setDirection(m_yaw, m_pitch);
}



void Camera::setView()
{
    m_camRight = glm::normalize(glm::cross(m_direction, m_camUp));
    m_view = glm::lookAt(m_position, m_position + m_direction, m_camUp);
}


void Camera::setDirection(float yaw, float pitch)
{
    m_yaw = yaw;
    m_pitch = pitch;
    // Don't allow camera to have angle outside of [-90, 90]
    if(pitch > 89) pitch = 89;
    if(pitch < -89) pitch = -89;

    float theta = glm::radians(pitch);
    float phi = glm::radians(yaw);

    m_direction.x = glm::cos(theta)*glm::sin(phi);
    m_direction.y = glm::sin(theta);
    m_direction.z = -glm::cos(theta)*glm::cos(phi);

    m_camRight = glm::normalize(glm::cross(m_direction, m_camUp));

}



} // end namespace seng

