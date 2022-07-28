//
// Created by jcollin2 on 7/26/22.
//

#ifndef SIM_ENGINE_LIGHTSYSTEMS_H
#define SIM_ENGINE_LIGHTSYSTEMS_H

#include "Misc/Util.h"

#include "Engine/Input.h"

#include "Systems/System.h"

namespace seng
{

class MovePtLight : public System
{
private:
    float m_panSpeed{1.f};
public:
    void StartUp() override{};

    void Update(float deltaTime) override;

    void ShutDown() override{};
};

} // seng

#endif //SIM_ENGINE_LIGHTSYSTEMS_H
