//
// Created by jcollin2 on 7/29/22.
//

#include "Scene/Material.h"

namespace seng
{

const std::string SolidMaterial::ambientUniformName{"ambientMat"};
const std::string SolidMaterial::diffuseUniformName{"diffuseMat"};
const std::string SolidMaterial::specularUniformName{"specularMat"};
const std::string SolidMaterial::roughnessUniformName{"roughness"};


/***************** SetupMaterial  ******************
 * @brief Set the uniforms for the color in the passed shader.
 *
 *     This assumed the shader program is already in use.
 *
 * @param shader Shader that holds the uniforms for color.
******************************************************************///
void SolidMaterial::SetupMaterial(Shader& shader)
{
    shader.setUniform1f(ambientUniformName, m_ambient);
    shader.setUniform3f(diffuseUniformName, m_diffuse.r, m_diffuse.g, m_diffuse.b);
    shader.setUniform3f(specularUniformName, m_specular.r, m_specular.g, m_specular.b);
    shader.setUniform1ui(roughnessUniformName, m_roughness);

}


}