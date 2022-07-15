//
// Created by jcollin2 on 7/1/22.
//

#include "Scene/SceneManager.h"
#include "ObjComp/Object.h"


using namespace seng;
/***************** StartUp  ******************
     * @brief Load the scene with objects.
     *      TODO: This will be deserialized from JSON file in future.
******************************************************************///
int seng::SceneManager::StartUp()
{
    //***********************************************************
    //       Create a bunch of objects and store them in the scene
    //***********************************************************
    Object obj{};



    return 1;
}

void SceneManager::ShutDown()
{
    //***********************************************************
    //       Possibly do nothing until serialization is implemented.
    //***********************************************************
}

void SceneManager::Update(float deltaTime)
{

}


