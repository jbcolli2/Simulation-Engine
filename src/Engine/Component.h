//
// Created by Jeb Collins on 2/25/22.
//

#ifndef seng_RENDERER_COMPONENT_HPP
#define seng_RENDERER_COMPONENT_HPP


#include <unordered_map>

#include "Object.h"

namespace seng
{

class Object;

class Component
{
public:
    Object* parentObject{nullptr};

    Component() = default;
    virtual ~Component() = default;

    virtual void StartUp(){};
    virtual void Update(){};
    virtual void FixedUpdate(){};
    virtual void ShutDown(){};
};



} // End namespace seng
#endif //seng_RENDERER_COMPONENT_HPP
