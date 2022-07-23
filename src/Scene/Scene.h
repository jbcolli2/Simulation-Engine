//
// Created by jcollin2 on 7/3/22.
//

#ifndef SIM_ENGINE_SCENE_H
#define SIM_ENGINE_SCENE_H

#include "ObjComp/Object.h"

#include "Rendering/Shader.hpp"
#include "Rendering/Renderable.h"

#include "Scene/Camera.h"
#include "Scene/System.h"



namespace seng
{


class Scene
{
private:
    std::vector<Object*> m_renderables{};
    std::vector<Object*> m_lights{};
    std::vector<Object*> m_cameras{};

    std::vector<System*> m_systems{};


public:
    /***************** StartUp  ******************
     * @brief Call startup on all systems.
    ******************************************************************///
    void StartUp();

    /***************** Update  ******************
     * @brief Update all objects in the scene.  Mostly done through
     *      calling update on all systems in the scene.
     *
     * @param deltaTime delta from last frame
    ******************************************************************///
    void Update(float deltaTime);

    /***************** ShutDown  ******************
     * @brief Clean up objects before they are deleted in SceneManager Shutdown.
    ******************************************************************///
    void ShutDown();




    /***************** AddObject  ******************
     * @brief Adds an object to the scene.  Sorts object into the correct object list of
     *      either renderables, a type of light, or camera.
     *
     * @param object Object to add.
    ******************************************************************///
    void AddObject(Object* object);


    /***************** DrawScene  ******************
     * @brief Call draw methods in all renderable objects.  Assume shader program is
     *      already in use.
    ******************************************************************///
    void DrawScene(Shader& shader);

    /***************** GetMainCamera  ******************
     * @brief Return a pointer to the main camera in the scene.
     *
     * @returns Main camera
    ******************************************************************///
    Camera* GetMainCamera()
    {
        return m_cameras[0]->GetComponent<Camera>();
    }

    void AddSystem(System* system)
    {
        m_systems.push_back(system);
    }
}; // end class Scene


} // end namespace seng

#endif //SIM_ENGINE_SCENE_H
