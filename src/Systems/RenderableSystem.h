//
// Created by jcollin2 on 7/23/22.
//

#ifndef SIM_ENGINE_RENDERABLESYSTEM_H
#define SIM_ENGINE_RENDERABLESYSTEM_H

#include "Misc/Util.h"

#include "Components/Renderable.h"

#include "Systems/System.h"

namespace seng
{

class RenderableSystem : public System
{
public:
    void StartUp() override {};
    void Update(float deltaTime) override;
    void ShutDown() override {};
};





class MoveObject : public System
{
private:
    float time{0.f};
    float m_amplitude{};
    float m_frequency{};
public:
    void StartUp() override;

    void Update(float deltaTime) override;

    void ShutDown() override {};

};

} // seng

#endif //SIM_ENGINE_RENDERABLESYSTEM_H
