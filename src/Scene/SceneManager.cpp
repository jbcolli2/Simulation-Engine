//
// Created by jcollin2 on 7/1/22.
//

#include "Misc/Util.h"

#include "Scene/SceneManager.h"
#include "Engine/Object.h"

#include "Scene/Cube.h"
#include "Scene/SolidMaterial.h"
#include "Systems/CameraController.h"


using namespace seng;
/***************** StartUp  ******************
     * @brief Load the scene with objects.
     *      TODO: This will be deserialized from JSON file in future.
******************************************************************///
int seng::SceneManager::StartUp(DisplayManager* displayManager)
{
    m_displayManager = displayManager;


    //***********************************************************
    //       Add objects to Scene
    //***********************************************************

    ///////////////// Create Meshes and Materials ///////////
    Cube* cubeMesh = &Cube::GetInstance();
    SolidMaterial* blueMat = new SolidMaterial();
    blueMat->m_diffuse = glm::vec3(0.2f, 0.f, 0.9f);
    cubeMesh->SetMaterial(blueMat);




    ///////////////// Create renderables ///////////////////////
    Object* cube = new Object();
    cube->GetTransform().position = glm::vec3(0.f, 0.f, -2.f);
    Renderable* tempRend = new Renderable();
    tempRend->m_meshes.push_back(cubeMesh);
    cube->AddComponent(tempRend);

    m_scene.AddObject(cube);

    /////////////////    Create camera    ///////////////////////
    Object* camera = new Object();
    camera->AddComponent(new Camera());
    Camera* cam = camera->GetComponent<Camera>();
    cam->SetDirection(-40, -30);
    cam->m_position = glm::vec3(1.5f, 1.5f, 0.f);
    m_scene.AddObject(camera);

    /////////////////    Create lights    ///////////////////////
    Object* dirLight = new Object();
    dirLight->AddComponent(new DirLight());
    m_scene.AddObject(dirLight);


    //***********************************************************
    //       Add Systems to Scene
    //***********************************************************
    CameraController* cameraController = new CameraController(m_displayManager);
    cameraController->AddObject(camera);
    m_scene.AddSystem(cameraController);

    RenderableSystem* renderSystem = new RenderableSystem();
    std::vector<Object*>& sceneRenderables = m_scene.GetRenderables();
    for(Object* renderObject : sceneRenderables)
    {
        renderSystem->AddObject(renderObject);
    }
    m_scene.AddSystem(renderSystem);





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


