//
// Created by jcollin2 on 7/19/22.
//

#include "Scene/Scene.h"

#include "Engine/Object.h"
#include "Systems/System.h"

#include "Components/Lights.h"
#include "Components/Camera.h"

#include "Rendering/Mesh.h"

namespace seng
{

/***************** StartUp  ******************
 * @brief Call startup on all systems.
******************************************************************///
void Scene::StartUp()
{
    m_ambientIntensity = .2f;
    for(Object* object : m_objects)
    {
        object->StartUp();
    }
    for(System* system : m_systems)
    {
        system->StartUp();
    }
    for(System* system : m_physSystems)
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
    for(Object* object : m_objects)
    {
        object->Update(deltaTime);
    }


    for(System* system : m_systems)
    {
        system->Update(deltaTime);
    }
}



void Scene::UpdatePhysics(float deltaTime)
{
    m_physTimer += deltaTime;

    while(m_physTimer > m_physTimeStep)
    {
        for(System* system : m_physSystems)
        {
            system->Update(m_physTimeStep);
        }

        m_physTimer -= m_physTimeStep;
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
    for(System* system : m_physSystems)
    {
        system->ShutDown();
    }
}






void Scene::AddObject(Object* object)
{
    m_objects.push_back(object);
    if(object->HasComponent<Camera>())
    {
        m_cameras.push_back(object);
    }
    if(object->HasComponent<DirLight>() || object->HasComponent<PointLight>())
    {
        m_lights.push_back(object);
    }
}









/***************** GetMainCamera  ******************
 * @brief Return a pointer to the main camera in the scene.
 *
 *      TODO: Update this to return a particular camera, not just
 *          the first one in the vector.
 *
 * @returns Main camera
******************************************************************///
Camera* Scene::GetMainCamera()
{
    return m_cameras[0]->GetComponent<Camera>();
}


} // end namespace seng