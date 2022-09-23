//
// Created by jcollin2 on 7/19/22.
//

#ifndef SIM_ENGINE_CAMERA_H
#define SIM_ENGINE_CAMERA_H

#include "Engine/DisplayManager.h"

#include "Engine/Component.h"

#include "Rendering/Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


namespace seng
{

class Camera : public Component
{
private:

    const static std::string positionUniformName;

    float m_yaw{0.f}, m_pitch{0.f};

    glm::vec3 m_direction{0.f, 0.f, -1.f};
    glm::vec3 m_camRight{1.f, 0.f,0.f}; // At beginning right is in pos x direction
    glm::vec3 m_camUp{0.f, 1.f, 0.f};  // Up is pos y, unless implement rolling


    /***************** angleToDirection  ******************
     * @brief Convert pitch and yaw into cartesian direction.
     *
     * @param yaw horizontal angle of direction in degrees [-90, 90]
     * @param pitch vertical angle of direction in degrees.  [-90, 90]
    ******************************************************************///
    glm::vec3 angleToDirection(float yaw, float pitch);

    void computeCamRight();

    void setView();
public:


    glm::vec3 m_position{0.f, 0.f, 0.f};

    float m_fov{45.f};  // Field of View
    float m_nearField{.1f};
    float m_farField{100.f};

    glm::mat4 m_view{1.f};
    glm::mat4 m_proj{1.f};


    /***************** Camera ctor  ******************
     * @brief Set the right vector and then set view matrix.
    ******************************************************************///
    Camera(DisplayManager* displayManager);
    Camera(const glm::vec3& position, const glm::vec3& direction, DisplayManager* displayManager);

    void IncrementDirection(float yawInc, float pitchInc);


    /***************** SetUniforms  ******************
     * @brief Set the uniforms on a shader with camera information.
     *      Right now this is just camera position for specular reflection.
     *
     * @param shader Shader being used that has uniforms.
    ******************************************************************///
    void SetUniforms(Shader& shader);


    //***********************************************************
    //       Getters/Setters
    //***********************************************************
    void SetDirection(float yaw, float pitch);
    // TODO: Include SetDirection with glm::vec3 as parameter.  Must make sure to set yaw and pitch when it is implemented.
    glm::vec3& GetDirection() {return m_direction;};
    glm::vec3& GetRight() {return m_camRight;};
    glm::vec3& GetUp() {return m_camUp;};
};








class CameraController : public Component
{
private:
    Camera& m_camera;           // the Camera component that is being controlled.  Must be passed in through ctor.

    float m_panSpeed;           // speed camera moves through space
    float m_rotateSpeed;        // speed camera changes view direction

    glm::vec2 m_oldMousePosition;   // prev mouse position to compute mouse position delta on each frame for rotating camera

    DisplayManager& m_dispManager;  // Need access to window to hide/show mouse cursor
public:
    /***************** CameraController ctor  ******************
     * @brief Pass in Camera component and initialize members.
     *
     *      oldMousePosition set to golden value outside of valid range to indicate not yet set.
    ******************************************************************///
    CameraController(DisplayManager& displayManager, Camera& camera, float panSpeed = 1.f, float rotateSpeed = 1.f) :
    m_camera(camera), m_panSpeed(panSpeed), m_rotateSpeed(rotateSpeed), m_oldMousePosition(glm::vec2(-1.f)),
    m_dispManager(displayManager){};

    /***************** StartUp  ******************
     * @brief Hide the cursor.
    ******************************************************************///
    void StartUp() override;
    void Update(float deltaTime) override;
};


} // end namespace seng









#endif //SIM_ENGINE_CAMERA_H
