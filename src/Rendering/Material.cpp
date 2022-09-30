//
// Created by jcollin2 on 7/29/22.
//

#include "Material.h"

namespace seng
{

std::unique_ptr<Material> Material::m_defaultMaterial{std::make_unique<SolidMaterial>(
        glm::vec3{.7f, 0.f, .4f}, glm::vec3{1.f}, 16
        )};

/***************** SetupMaterial  ******************
 * @brief Set the uniforms for the color in the passed shader.
 *
 *     This assumed the shader program is already in use.
 *
 * @param shader Shader that holds the uniforms for color.
******************************************************************///
void SolidMaterial::SetupMaterial(Shader& shader)
{
    shader.m_solidMatComp.SetUniform(*this);
}


void SolidMaterial::ResetMaterial(Shader& shader)
{
    shader.m_solidMatComp.ResetUniform();
}


//***********************************************************
//       Texture Material
//***********************************************************
TextureMaterial::TextureMaterial(const std::string& path, unsigned int texUnit, const glm::vec3& specColor,
                                 float roughness) :
    m_specColor(specColor), m_roughness(roughness)
{
    m_diffTexture.m_fileName = path;
    m_diffTexture.m_texUnit = texUnit;
    m_diffTexture.m_tbo = GenAndLoadTBO(path);

}

void TextureMaterial::SetupMaterial(Shader& shader)
{
    glActiveTexture(GL_TEXTURE0 + m_diffTexture.m_texUnit);
    glBindTexture(GL_TEXTURE_2D, m_diffTexture.m_tbo);
    shader.m_textureMatComp.SetUniform(*this);
}


void TextureMaterial::ResetMaterial(Shader& shader)
{
    shader.m_textureMatComp.ResetUniform();
    glBindTexture(GL_TEXTURE_2D, 0);
}

}