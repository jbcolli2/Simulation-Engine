//
// Created by jcollin2 on 7/22/22.
//

#ifndef SIM_ENGINE_CAMERACONTROLLER_H
#define SIM_ENGINE_CAMERACONTROLLER_H

#include "Scene/System.h"


namespace seng
{

class CameraController : public System
{
public:
    void virtual StartUp() override;
    void virtual Update(float deltaTime) override;
    void virtual ShutDown() override{};
};

} // seng

#endif //SIM_ENGINE_CAMERACONTROLLER_H
