//
// Created by jcollin2 on 7/13/22.
//

#include <iostream>

#include "ObjComp/Object.h"
#include "ObjComp/Transform.h"
#include "Rendering/Renderable.h"





using namespace seng;

int main()
{

    Object obj{};
    Transform* transform = new Transform();
    transform->position = glm::vec3(3.f, 5.f, 8.f);
    transform->rotation = glm::vec3(.2f, -.3f, -.99f);

    obj.AddComponent(transform);
    obj.AddComponent(new Renderable());
    Transform* newTrans = obj.GetComponent<Transform>();
    Renderable* temp = obj.GetComponent<Renderable>();

    if(obj.HasComponent<Transform>())
    {
        std::cout << "Has Transform\n";
    }
    if(!obj.HasComponent<Renderable>())
    {
        std::cout << "Doesn't have renderable\n";
    }
    if(obj.HasSignature(ComponentManager::GetSignature<Transform>()))
    {
        std::cout << "Has transform sig\n";
    }

    std::cout << newTrans->position.x << ", " << newTrans->position.y << ".\n";

}