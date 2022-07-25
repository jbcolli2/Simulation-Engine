//
// Created by jcollin2 on 7/22/22.
//

#ifndef SIM_ENGINE_CAMERACONTROLLER_H
#define SIM_ENGINE_CAMERACONTROLLER_H

#include "Misc/Util.h"

#include "Engine/Input.h"

#include "Systems/System.h"
#include "Components/Camera.h"


namespace seng
{

class CameraController : public System
{
private:
    DisplayManager* m_displayManager{};

    glm::vec2 m_oldMousePosition;
    float m_rotateSpeed{1.f};
    float m_panSpeed{1.f};

    Camera* m_camera{};
public:
    CameraController(DisplayManager* displayManager) : m_displayManager(displayManager){};
    void virtual StartUp() override;
    void virtual Update(float deltaTime) override;
    void virtual ShutDown() override{};
};

} // seng

#endif //SIM_ENGINE_CAMERACONTROLLER_H
