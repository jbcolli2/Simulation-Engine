//
// Created by jcollin2 on 7/19/22.
//


#include "Scene/Camera.h"

namespace seng
{
Camera::Camera()
{
    setView();
}

Camera::Camera(const glm::vec3& position, const glm::vec3& direction) :
    m_position(position), m_direction(direction)
{
    setView();
}




void Camera::setView()
{
    m_camRight = glm::normalize(glm::cross(m_direction, m_camUp));
    m_view = glm::lookAt(m_position, m_position + m_direction, m_camUp);
}


} // end namespace seng

