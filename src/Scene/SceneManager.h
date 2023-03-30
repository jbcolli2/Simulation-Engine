//
// Created by jcollin2 on 7/1/22.
//

#ifndef SIM_ENGINE_SCENEMANAGER_H
#define SIM_ENGINE_SCENEMANAGER_H

#include "Scene/Scene.h"

namespace seng
{
class DisplayManager;
class SceneManager
{
private:
    DisplayManager* m_displayManager;



public:

    Scene m_scene{};

    /***************** StartUp  ******************
         * @brief Load the scene with objects.
         *      TODO: This will be deserialized from JSON file in future.
    ******************************************************************///
    virtual int StartUp(DisplayManager* displayManager);

    /***************** ShutDown  ******************
         * @brief Clean up all objects in the scene.
         *      TODO: In future, give option to serialize scene.
    ******************************************************************///
    void ShutDown();

    /***************** Update  ******************
         * @brief Update all objects in the current scene.  This could include moving them due
         *  to input, or moving them due to physics.  This will certainly include updating the camera
         *  position and possibly the light positions.  Could also change color of the lights
         *  if they depend on time in some procedural way.
         *
         * @param deltaTime Amount of game time that has elapsed since last frame.
    ******************************************************************///
    void Update(float deltaTime, float physDeltaTime);



    /***************** Object Factory Methods  ******************
     * @brief Theses create objects on the heap, add the proper components and set them up based on
     *      parameters of the method.  Then adds them to the scene.
    ******************************************************************///
    void AddEmptyObject(std::string id, const Transform& transform = Transform());
    void AddCube(std::string id, std::string materialID = "", const Transform& transform = Transform());
    void AddPlane(std::string id, std::string materialID = "", const Transform& transform = Transform());
    void AddSphere(std::string id, std::string materialID = "", unsigned int subdivideIters = 4, const Transform& transform = Transform());
    void AddModel(std::string id, std::string modelID, const Transform& transform = Transform());
    void AddPointLight(std::string id, glm::vec3 position, bool movable, float diffIntensity = .7f, float specIntensity = .4f,
                       glm::vec3 color = glm::vec3(1.f));
    void AddDirLight(std::string id, glm::vec3 direction, float diffIntensity = .9f, float specIntensity = .1f,
                     glm::vec3 color = glm::vec3(1.f));
    void AddCamera(std::string id, glm::vec3 position, glm::vec3 direction = glm::vec3(0.f, 0.f, -1.f), bool controllable = true);

}; //End SceneManager






class PrimScene1 : public SceneManager
{
public:
    int StartUp(DisplayManager* displayManager) override;
};




} //End namespace seng



#endif //SIM_ENGINE_SCENEMANAGER_H
