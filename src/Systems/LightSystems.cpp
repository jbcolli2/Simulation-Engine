//
// Created by jcollin2 on 7/26/22.
//

#include "Systems/LightSystems.h"
#include "Components/Lights.h"


namespace seng
{

void MovePtLight::Update(float deltaTime)
{
    float speed = m_panSpeed*deltaTime;
    PointLight* pointLight = m_objects[0]->GetComponent<PointLight>();
    Transform& trans = m_objects[0]->GetTransform();


    if(Input::GetInstance().KeyPress(GLFW_KEY_I))
    {
        pointLight->m_position.z -= speed;
    }
    if(Input::GetInstance().KeyPress(GLFW_KEY_K))
    {
        pointLight->m_position.z += speed;
    }
    if(Input::GetInstance().KeyPress(GLFW_KEY_J))
    {
        pointLight->m_position.x -= speed;
    }
    if(Input::GetInstance().KeyPress(GLFW_KEY_L))
    {
        pointLight->m_position.x += speed;
    }
    if(Input::GetInstance().KeyPress(GLFW_KEY_U))
    {
        pointLight->m_position.y -= speed;
    }
    if(Input::GetInstance().KeyPress(GLFW_KEY_O))
    {
        pointLight->m_position.y += speed;
    }

    trans.position = pointLight->m_position;
}

} // seng