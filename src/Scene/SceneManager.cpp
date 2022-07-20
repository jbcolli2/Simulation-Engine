//
// Created by jcollin2 on 7/1/22.
//

#include "Misc/Util.h"

#include "Scene/SceneManager.h"
#include "ObjComp/Object.h"

#include "Scene/Cube.h"
#include "Scene/SolidMaterial.h"


using namespace seng;
/***************** StartUp  ******************
     * @brief Load the scene with objects.
     *      TODO: This will be deserialized from JSON file in future.
******************************************************************///
int seng::SceneManager::StartUp()
{
    ///////////////// Create Meshes and Materials ///////////
    Cube cubeMesh{};
    SolidMaterial blueMat{};
    blueMat.m_diffuse = glm::vec3(0.2f, 0.f, 0.9f);
    cubeMesh.SetMaterial(&blueMat);




    ///////////////// Create game objects ///////////////////////
    Object* cube = new Object();
    Renderable* tempRend = new Renderable();
    tempRend->m_meshes.push_back(&cubeMesh);
    cube->AddComponent(tempRend);

    m_scene.AddObject(cube);

    Object* camera = new Object();
    camera->AddComponent(new Camera());
    m_scene.AddObject(camera);




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


