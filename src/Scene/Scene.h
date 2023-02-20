//
// Created by jcollin2 on 7/3/22.
//

#ifndef SIM_ENGINE_SCENE_H
#define SIM_ENGINE_SCENE_H

#include "Misc/Common.h"

#include "Rendering/Material.h"



namespace seng
{
class Object;
class System;
class Shader;
class Camera;
class Scene
{
private:
    std::unordered_map<std::string, int> m_objectID{};
    std::unordered_map<std::string, std::unique_ptr<Material>> m_materialList;

    std::vector<unsigned int> m_lights{};                        // Convenience list of all light objects
    std::vector<unsigned int> m_cameras{};                       // Convenience  list of all camera objects
    std::vector<std::unique_ptr<Object>> m_objects;         // List of all objects in scene, even lights and cameras

    float m_physTimer{0.f};                                 // Use to keep fixed time step for physics objects



public:
    float m_physTimeStep{.016f};

    float m_ambientIntensity{.2f};

    /***************** Scene Ctor  ******************
     * @brief Initialize all variables, but especially m_materialList and m_objects.
     *      Initialize them in ctor so that we do not have to include Object and Material
     *      dependencies in the Scene header file.
    ******************************************************************///
    Scene();

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
    void ShutDown(){};


    //***********************************************************
    //       Adders
    //***********************************************************

    /***************** AddObject  ******************
     * @brief Adds an object to the scene.  Sorts object into the correct object list of
     *      either renderables, a type of light, or camera.
     *
     * @param object Object to add.
    ******************************************************************///
    void AddObject(std::unique_ptr<Object>&& object);


    /***************** AddMaterial  ******************
     * @brief Material is passed as r-reference b/c type information is not retained for
     *      materials.  Materials are fully defined at their creation and then ownership is passed
     *      to the scene.
     *
     * @param name String identifier for the material
     * @param material Previous owner of the material, pointer that owned it during creation so as to
     *          setup the material while type information is known.  Once ownership is transfered to the
     *          scene, type info is gone and the material can no longer be changed.
    ******************************************************************///
    void AddMaterial(const std::string& name, std::unique_ptr<Material>&& material);
    /***************** AddMaterial  ******************
     * @brief Add material by passing in r-value pointer (i.e. new SolidMaterial()).
     *
     * @param name string id
     * @param material r-value pointer to material (new Material())
    ******************************************************************///
    void AddMaterial(const std::string& name, Material*&& material);







    //***********************************************************
    //       Getters/Setters
    //***********************************************************

    /***************** GetMainCamera  ******************
     * @brief Return a pointer to the main camera in the scene.
     *
     * @returns Main camera
    ******************************************************************///
    Camera& GetMainCamera();
    
    /***************** GetMaterial  ******************
     * @brief Return raw pointer to material by string id.
     * 
     * @param id String id of material
     * 
     * @returns Raw pointer to material owned by scene.
    ******************************************************************///
    Material* GetMaterial(std::string id);

    /***************** FindObjectByID  ******************
     * @brief Returns reference to object by it's string id.
     *
     * @param id Unique identifier for the object.
     *
     * @returns Reference to object.
    ******************************************************************///
    Object& FindObjectByID(std::string id);


    friend class Renderer;
}; // end class Scene


} // end namespace seng

#endif //SIM_ENGINE_SCENE_H
