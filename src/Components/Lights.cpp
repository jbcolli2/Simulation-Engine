//
// Created by jcollin2 on 7/24/22.
//

#include "Lights.h"

namespace seng
{


//***********************************************************
//       Point Light Class
//***********************************************************
const std::string PointLight::positionUniformName{"ptLightPosition"};
const std::string PointLight::colorUniformName{"ptLightColor"};

void PointLight::SetUniforms(Shader& shader)
{
    shader.setUniform3f(positionUniformName, m_position.x, m_position.y, m_position.z);
    shader.setUniform3f(colorUniformName, m_color.r, m_color.g, m_color.b);
}







//***********************************************************
//       Direction Light Class
//***********************************************************
const std::string DirLight::directionUniformName{"dirLightDirection"};
const std::string DirLight::colorUniformName{"dirLightColor"};


void DirLight::SetUniforms(Shader& shader)
{
    shader.setUniform3f(directionUniformName, m_direction.x, m_direction.y, m_direction.z);
    shader.setUniform3f(colorUniformName, m_color.r, m_color.g, m_color.b);
}
} // seng