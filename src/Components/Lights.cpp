//
// Created by jcollin2 on 7/24/22.
//

#include "Lights.h"

#include "Engine/Input.h"

namespace seng
{


//***********************************************************
//       Point Light Class
//***********************************************************
const std::string PointLight::positionUniformName{"pointLight.position"};
const std::string PointLight::colorUniformName{"pointLight.color"};
const std::string PointLight::diffIntensityUniformName{"pointLight.diffuseIntensity"};
const std::string PointLight::specIntensityUniformName{"pointLight.specularIntensity"};

void PointLight::SetUniforms(Shader& shader)
{
    shader.m_pointLightComp.SetUniform(*this);
}







//***********************************************************
//       Direction Light Class
//***********************************************************
const std::string DirLight::directionUniformName{"dirLight.direction"};
const std::string DirLight::colorUniformName{"dirLight.color"};
const std::string DirLight::diffIntensityUniformName{"dirLight.diffuseIntensity"};
const std::string DirLight::specIntensityUniformName{"dirLight.specularIntensity"};


void DirLight::SetUniforms(Shader& shader)
{
    shader.m_dirLightComp.SetUniform(*this);
}








//***********************************************************
//       Move Point Light Methods
//***********************************************************

void MovePtLight::Update(float deltaTime)
{
    float speed = m_speed*deltaTime;
    Transform& trans = parentObject->GetTransform();


    if(Input::GetInstance().KeyPress(GLFW_KEY_I))
    {
        m_pointLight.m_position.z -= speed;
    }
    if(Input::GetInstance().KeyPress(GLFW_KEY_K))
    {
        m_pointLight.m_position.z += speed;
    }
    if(Input::GetInstance().KeyPress(GLFW_KEY_J))
    {
        m_pointLight.m_position.x -= speed;
    }
    if(Input::GetInstance().KeyPress(GLFW_KEY_L))
    {
        m_pointLight.m_position.x += speed;
    }
    if(Input::GetInstance().KeyPress(GLFW_KEY_U))
    {
        m_pointLight.m_position.y -= speed;
    }
    if(Input::GetInstance().KeyPress(GLFW_KEY_O))
    {
        m_pointLight.m_position.y += speed;
    }

    trans.position = m_pointLight.m_position;

}
} // seng