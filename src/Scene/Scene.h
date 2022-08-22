//
// Created by jcollin2 on 7/3/22.
//

#ifndef SIM_ENGINE_SCENE_H
#define SIM_ENGINE_SCENE_H

#include "Engine/Object.h"

#include "Rendering/Shader.hpp"
#include "Components/Renderable.h"

#include "Components/Camera.h"
#include "Components/Lights.h"
#include "Components/Lights.h"
#include "Systems/System.h"



namespace seng
{


class Scene
{
private:
    std::vector<Object*> m_renderables{};
    std::vector<Object*> m_lights{};
    std::vector<Object*> m_cameras{};

    std::vector<System*> m_systems{};
    std::vector<System*> m_physSystems{};

    float m_physTimer{0.f};



public:
    float m_physTimeStep{.016f};

    float m_ambientIntensity{.2f};

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

    void UpdatePhysics(float deltaTime);

    /***************** ShutDown  ******************
     * @brief Clean up objects before they are deleted in SceneManager Shutdown.
    ******************************************************************///
    void ShutDown();


    //***********************************************************
    //       Adders
    //***********************************************************

    /***************** AddObject  ******************
     * @brief Adds an object to the scene.  Sorts object into the correct object list of
     *      either renderables, a type of light, or camera.
     *
     * @param object Object to add.
    ******************************************************************///
    void AddObject(Object* object);

    void AddSystem(System* system)
    {
        m_systems.push_back(system);
    }

    void AddPhysicsSystem(System* system)
    {
        m_physSystems.push_back(system);
    }

    /***************** DrawScene  ******************
     * @brief Call draw methods in all renderable objects.  Assume shader program is
     *      already in use.
    ******************************************************************///
    void DrawScene(Shader& shader);



    //***********************************************************
    //       Getters/Setters
    //***********************************************************

    /***************** GetMainCamera  ******************
     * @brief Return a pointer to the main camera in the scene.
     *
     * @returns Main camera
    ******************************************************************///
    Camera* GetMainCamera()
    {
        return m_cameras[0]->GetComponent<Camera>();
    }

    std::vector<Object*>& GetRenderables() {return m_renderables;};


}; // end class Scene


} // end namespace seng

#endif //SIM_ENGINE_SCENE_H
