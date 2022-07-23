//
// Created by jcollin2 on 7/19/22.
//

#ifndef SIM_ENGINE_CAMERA_H
#define SIM_ENGINE_CAMERA_H

#include "Engine/DisplayManager.h"

#include "ObjComp/Component.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace seng
{

class Camera : public Component
{
private:
    glm::vec3 m_camRight{1.f, 0.f,0.f}; // At beginning right is in pos x direction
    glm::vec3 m_camUp{0.f, 1.f, 0.f};  // Up is pos y, unless implement rolling
    float m_yaw{0.f}, m_pitch{0.f};

    /***************** angleToDirection  ******************
     * @brief Convert pitch and yaw into cartesian direction.
     *
     * @param yaw horizontal angle of direction in degrees [-90, 90]
     * @param pitch vertical angle of direction in degrees.  [-90, 90]
    ******************************************************************///
    void setDirection(float yaw, float pitch);
public:
    // TODO: Move back to private.
    void setView();

    glm::vec3 m_position{0.f, 0.f, 0.f};
    glm::vec3 m_direction{0.f, 0.f, -1.f};
    float m_fov{45.f};  // Field of View
    float m_nearField{.1f};
    float m_farField{100.f};

    glm::mat4 m_view{1.f};
    glm::mat4 m_proj{1.f};


    /***************** Camera ctor  ******************
     * @brief Set the right vector and then set view matrix.
    ******************************************************************///
    Camera();
    Camera(const glm::vec3& position, const glm::vec3& direction);

    void IncrementDirection(float yawInc, float pitchInc);



};



} // end namespace seng









#endif //SIM_ENGINE_CAMERA_H
