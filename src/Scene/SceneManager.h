//
// Created by jcollin2 on 7/1/22.
//

#ifndef SIM_ENGINE_SCENEMANAGER_H
#define SIM_ENGINE_SCENEMANAGER_H

#include "Scene/Scene.h"
#include "Engine/Input.h"

#include "ObjComp/Object.h"

namespace seng
{
class SceneManager
{
public:
    Scene m_scene;

    /***************** StartUp  ******************
         * @brief Load the scene with objects.
         *      TODO: This will be deserialized from JSON file in future.
    ******************************************************************///
    int StartUp();

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
    void Update(float deltaTime);

}; //End SceneManager
} //End namespace seng



#endif //SIM_ENGINE_SCENEMANAGER_H
