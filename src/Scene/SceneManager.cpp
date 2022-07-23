//
// Created by jcollin2 on 7/1/22.
//

#include "Misc/Util.h"

#include "Scene/SceneManager.h"
#include "ObjComp/Object.h"

#include "Scene/Cube.h"
#include "Scene/SolidMaterial.h"
#include "Scene/CameraController.h"


using namespace seng;
/***************** StartUp  ******************
     * @brief Load the scene with objects.
     *      TODO: This will be deserialized from JSON file in future.
******************************************************************///
int seng::SceneManager::StartUp()
{
    ///////////////// Create Meshes and Materials ///////////
    Cube* cubeMesh = &Cube::GetInstance();
    SolidMaterial* blueMat = new SolidMaterial();
    blueMat->m_diffuse = glm::vec3(0.2f, 0.f, 0.9f);
    cubeMesh->SetMaterial(blueMat);




    ///////////////// Create game objects ///////////////////////
    Object* cube = new Object();
    Renderable* tempRend = new Renderable();
    tempRend->m_meshes.push_back(cubeMesh);
    tempRend->m_model = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -2.f));
    cube->AddComponent(tempRend);

    m_scene.AddObject(cube);

    Object* camera = new Object();
    camera->AddComponent(new Camera());
    Camera* cam = camera->GetComponent<Camera>();
    cam->m_direction = glm::vec3(-.3, -.2f, -1.f);
    cam->m_position = glm::vec3(.5f, .5f, 0.f);
    cam->setView();
    m_scene.AddObject(camera);

    CameraController* cameraController = new CameraController();
    cameraController->AddObject(camera);
    m_scene.AddSystem(cameraController);



    m_scene.StartUp();

    return 1;
}

void SceneManager::ShutDown()
{
    //***********************************************************
    //       Possibly do nothing until serialization is implemented.
    //***********************************************************
    m_scene.ShutDown();
}

void SceneManager::Update(float deltaTime)
{
    m_scene.Update(deltaTime);
}


