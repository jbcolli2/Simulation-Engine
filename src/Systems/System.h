//
// Created by jcollin2 on 7/22/22.
//

#ifndef SIM_ENGINE_SYSTEM_H
#define SIM_ENGINE_SYSTEM_H

#include "Misc/Util.h"
#include "Engine/Object.h"

namespace seng
{
class System
{
protected:
    Signature m_signature{};
    std::vector<Object*> m_objects{};

public:
    virtual void StartUp() = 0;

    virtual void Update(float deltaTime) = 0;

    virtual void ShutDown() = 0;

    void AddObject(Object* object)
    {
        m_objects.push_back(object);
    }
    void SetSignature(Signature signature)
    {
        m_signature = signature;
    }
};
}






#endif //SIM_ENGINE_SYSTEM_H
