//
// Created by jcollin2 on 7/3/22.
//

#ifndef SIM_ENGINE_SCENE_H
#define SIM_ENGINE_SCENE_H



#include "Misc/Common.h"


namespace seng
{

class Object;
class Material;
class System;
class Shader;
class Camera;
class Scene
{
private:
    std::unordered_map<std::string, Material*> m_materialList{};

    std::vector<Object*> m_lights{};
    std::vector<Object*> m_cameras{};
    std::vector<Object*> m_objects{};

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

    void AddMaterial(const std::string& name, Material* material)
    {
        m_materialList[name] = material;
    }

    void AddSystem(System* system)
    {
        m_systems.push_back(system);
    }

    void AddPhysicsSystem(System* system)
    {
        m_physSystems.push_back(system);
    }





    //***********************************************************
    //       Getters/Setters
    //***********************************************************

    /***************** GetMainCamera  ******************
     * @brief Return a pointer to the main camera in the scene.
     *
     * @returns Main camera
    ******************************************************************///
    Camera* GetMainCamera();


    friend class Renderer;
}; // end class Scene


} // end namespace seng

#endif //SIM_ENGINE_SCENE_H
