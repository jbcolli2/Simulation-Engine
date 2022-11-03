//
// Created by jcollin2 on 7/24/22.
//

#ifndef SIM_ENGINE_LIGHTS_H
#define SIM_ENGINE_LIGHTS_H

#include "Engine/Component.h"

#include "Rendering/Shader.h"


namespace seng
{

//***********************************************************
//       Point Light
//***********************************************************
class PointLight : public Component
{
private:
    const static std::string positionUniformName;
    const static std::string colorUniformName;
    const static std::string diffIntensityUniformName;
    const static std::string specIntensityUniformName;


public:
    glm::vec3 m_position{0.f};
    glm::vec3 m_color{1.f};
    float m_diffuseIntensity{.7f};
    float m_specularIntensity{.5f};


    PointLight() = default;
    PointLight(const glm::vec3& position, const glm::vec3& color = glm::vec3(1.f)) : m_position(position), m_color(color) {};

    /***************** SetUniforms  ******************
     * @brief Set the uniform variables for the point light.
     *
     * @param shader The shader in use that has uniforms to set.
    ******************************************************************///
    void SetUniforms(Shader& shader);
};




//***********************************************************
//       Directional Light
//***********************************************************
class DirLight : public Component
{
private:
    const static std::string directionUniformName;
    const static std::string colorUniformName;
    const static std::string diffIntensityUniformName;
    const static std::string specIntensityUniformName;



public:
    glm::vec3 m_direction{.1f, -1.f, -.1f};
    glm::vec3 m_color{1.f};
    float m_diffuseIntensity{.3f};
    float m_specularIntensity{.2f};

    DirLight() = default;
    DirLight(const glm::vec3& direction, const glm::vec3& color = glm::vec3(1.f)) :
            m_direction(direction), m_color(color) {};

    /***************** SetUniforms  ******************
     * @brief Set the uniform variables for the point light.
     *
     * @param shader The shader in use that has uniforms to set.
    ******************************************************************///
    void SetUniforms(Shader& shader);
};









//***********************************************************
//       Move Point Light Component
//***********************************************************
class MovePtLight : public Component
{
private:
    float m_speed;                  // Speed at which light is moved

    PointLight& m_pointLight;        // Ref to the light being moved

public:
    MovePtLight(PointLight& pointLight, float speed = 1.f) :
    m_pointLight{pointLight}, m_speed{speed}    {};

    void Update(float deltaTime) override;



    //***********************************************************
    //       Getter/Setter
    //***********************************************************
    void Speed(float speed)
    {
        m_speed = speed;
    }
};

} // seng

#endif //SIM_ENGINE_LIGHTS_H
