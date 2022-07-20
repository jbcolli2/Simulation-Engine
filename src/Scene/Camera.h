//
// Created by jcollin2 on 7/19/22.
//

#ifndef SIM_ENGINE_CAMERA_H
#define SIM_ENGINE_CAMERA_H

#include "ObjComp/Component.h"

#include <glm/ext/matrix_transform.hpp>


namespace seng
{

class Camera : public Component
{
private:
    glm::vec3 m_camRight{1.f, 0.f,0.f}; // At beginning right is in pos x direction
    glm::vec3 m_camUp{0.f, 1.f, 0.f};  // Up is pos y, unless implement rolling

    void setView();
public:
    glm::vec3 m_position{0.f, 0.f, 0.f};
    glm::vec3 m_direction{0.f, 0.f, -1.f};
    float m_fov{45.f};  // Field of View
    float m_nearField{.1f};
    float m_farField{100.f};

    glm::mat4 m_view{1.f};


    Camera();
    Camera(const glm::vec3& position, const glm::vec3& direction);

    void IncrementDirection(float yawInc, float pitchInc);



};



} // end namespace seng









#endif //SIM_ENGINE_CAMERA_H
