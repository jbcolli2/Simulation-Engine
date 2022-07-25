//
// Created by jcollin2 on 7/19/22.
//

#ifndef SIM_ENGINE_SOLIDMATERIAL_H
#define SIM_ENGINE_SOLIDMATERIAL_H

#include "Misc/Util.h"

#include "Scene/Material.h"

namespace seng
{

class SolidMaterial : public Material
{
private:
    const static std::string ambientUniformName;
    const static std::string diffuseUniformName;
    const static std::string specularUniformName;
    const static std::string roughnessUniformName;
public:
    // Color information
    glm::vec3 m_diffuse{1.f, 0.f, .3f};
    float m_ambient = .2f;
    glm::vec3 m_specular{1.f, 1.f, 1.f};
    unsigned int m_roughness{1};
    
    
    /***************** SolidMaterial ctor  ******************
     * @brief Set the color properties.
    ******************************************************************///
    SolidMaterial() = default;

    SolidMaterial(float ambient, const glm::vec3& diffuse, const glm::vec3& specular, unsigned int roughness) :
        m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_roughness(roughness) {};


    /***************** SetupMaterial  ******************
     * @brief Set the uniforms for the color in the passed shader.
     *
     *     This assumed the shader program is already in use.
     *
     * @param shader Shader that holds the uniforms for color.
    ******************************************************************///
    void SetupMaterial(Shader& shader) override;

};

} // seng

#endif //SIM_ENGINE_SOLIDMATERIAL_H
