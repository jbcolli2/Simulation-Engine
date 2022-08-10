//
// Created by jcollin2 on 8/9/22.
//

#ifndef SIM_ENGINE_CLOTHROD_H
#define SIM_ENGINE_CLOTHROD_H


#include "Systems/System.h"


namespace seng
{


class ClothRod : public System
{
public:
    void StartUp() override;

    void Update(float deltaTime) override;

    void ShutDown() override;
};

} // seng

#endif //SIM_ENGINE_CLOTHROD_H
