//
// Created by jcollin2 on 7/18/22.
//

#ifndef SIM_ENGINE_MATERIAL_H
#define SIM_ENGINE_MATERIAL_H

#include "Rendering/Shader.h"
#include "Engine/Component.h"

namespace seng
{

struct Texture
{
    unsigned int m_tbo{0};
    std::string m_fileName{""};
    unsigned int m_texUnit{0};
};





class Material
{

public:
    virtual ~Material() = default;

    virtual void SetupMaterial(Shader& shader) = 0;
    virtual void ResetMaterial(Shader& shader){};

    static std::unique_ptr<Material> m_defaultMaterial;
};




class SolidMaterial : public Material
{
public:
    // Color information
    glm::vec3 m_diffuse{1.f, 0.f, .3f};
    glm::vec3 m_specular{1.f, 1.f, 1.f};
    unsigned int m_roughness{1};


    /***************** SolidMaterial ctor  ******************
     * @brief Set the color properties.
    ******************************************************************///
    SolidMaterial() = default;

    SolidMaterial(const glm::vec3& diffuse, const glm::vec3& specular, unsigned int roughness) :
            m_diffuse(diffuse), m_specular(specular), m_roughness(roughness) {};


    /***************** SetupMaterial  ******************
     * @brief Set the uniforms for the color in the passed shader.
     *
     *     This assumed the shader program is already in use.
     *
     * @param shader Shader that holds the uniforms for color.
    ******************************************************************///
    void SetupMaterial(Shader& shader) override;


    /***************** ResetMaterial  ******************
     * @brief Reset flags for solid material.
     *
     * @param shader Shader objects containing flag uniforms.
    ******************************************************************///
    void ResetMaterial(Shader& shader) override;


};







class TextureMaterial : public Material
{
public:
    Texture m_diffTexture{};
    glm::vec3 m_specColor{1.f};
    float m_roughness{1.f};


    TextureMaterial() = default;
    TextureMaterial(const std::string& path, unsigned int texUnit, const glm::vec3& specColor = glm::vec3(1.f),
                    float roughness = 1.f);


    void SetupMaterial(Shader &shader) override;


    void ResetMaterial(Shader& shader) override;
};

} //end namespace seng


#endif //SIM_ENGINE_MATERIAL_H
