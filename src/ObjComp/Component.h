//
// Created by Jeb Collins on 2/25/22.
//

#ifndef seng_RENDERER_COMPONENT_HPP
#define seng_RENDERER_COMPONENT_HPP


#include <unordered_map>

#include "ObjComp/Object.h"

namespace seng
{

class Object;

class Component
{
public:
    Component() = default;

    virtual ~Component() = default;

    Object* parentObject{nullptr};
};



} // End namespace seng
#endif //seng_RENDERER_COMPONENT_HPP
