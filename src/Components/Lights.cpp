//
// Created by jcollin2 on 7/24/22.
//

#include "Lights.h"

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
    shader.setUniform3f(positionUniformName, m_position.x, m_position.y, m_position.z);
    shader.setUniform3f(colorUniformName, m_color.r, m_color.g, m_color.b);
    shader.setUniform1f(diffIntensityUniformName, m_diffuseIntensity);
    shader.setUniform1f(specIntensityUniformName, m_specularIntensity);
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
    shader.setUniform3f(directionUniformName, m_direction.x, m_direction.y, m_direction.z);
    shader.setUniform3f(colorUniformName, m_color.r, m_color.g, m_color.b);
    shader.setUniform1f(diffIntensityUniformName, m_diffuseIntensity);
    shader.setUniform1f(specIntensityUniformName, m_specularIntensity);
}
} // seng