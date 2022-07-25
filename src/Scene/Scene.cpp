//
// Created by jcollin2 on 7/19/22.
//

#include "Scene/Scene.h"

namespace seng
{

/***************** StartUp  ******************
 * @brief Call startup on all systems.
******************************************************************///
void Scene::StartUp()
{

    for(System* system : m_systems)
    {
        system->StartUp();
    }
}




/***************** Update  ******************
 * @brief Update all objects in the scene.  Mostly done through
 *      calling update on all systems in the scene.
 *
 * @param deltaTime delta from last frame
******************************************************************///
void Scene::Update(float deltaTime)
{
    for(System* system : m_systems)
    {
        system->Update(deltaTime);
    }
}




/***************** ShutDown  ******************
 * @brief Clean up objects before they are deleted in SceneManager Shutdown.
******************************************************************///
void Scene::ShutDown()
{
    for(System* system : m_systems)
    {
        system->ShutDown();
    }
}






void Scene::AddObject(Object* object)
{
    if(object->HasComponent<Renderable>())
    {
        m_renderables.push_back(object);
    }
    if(object->HasComponent<Camera>())
    {
        m_cameras.push_back(object);
    }
    if(object->HasComponent<DirLight>() || object->HasComponent<PointLight>())
    {
        m_lights.push_back(object);
    }
}




void Scene::DrawScene(Shader& shader)
{
    GetMainCamera()->SetUniforms(shader);
    for(Object* light : m_lights)
    {
        if(light->HasComponent<PointLight>())
            light->GetComponent<PointLight>()->SetUniforms(shader);
        if(light->HasComponent<DirLight>())
            light->GetComponent<DirLight>()->SetUniforms(shader);
    }
    for(Object* renderables : m_renderables)
    {
        renderables->GetComponent<Renderable>()->Draw(shader);
    }
}

} // end namespace seng