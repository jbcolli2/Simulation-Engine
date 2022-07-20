//
// Created by jcollin2 on 7/19/22.
//

#include "Scene/Scene.h"

namespace seng
{

void Scene::AddObject(Object* object)
{
    if(object->HasComponent<Renderable>())
    {
        m_renderables.push_back(object);
    }
}




void Scene::DrawScene(Shader& shader)
{
    for(Object* renderables : m_renderables)
    {
        renderables->GetComponent<Renderable>()->Draw();
    }
}

} // end namespace seng