//
// Created by jcollin2 on 7/1/22.
//

#include "Misc/Common.h"

#include "Scene/SceneManager.h"
#include "Engine/Object.h"

#include "Rendering/Material.h"

#include "Components/MeshComponents.h"
#include "Components/Lights.h"
#include "Components/Camera.h"


namespace seng
{


/***************** StartUp  ******************
     * @brief Load the scene with objects.
     *      TODO: This will be deserialized from JSON file in future.
******************************************************************///
int seng::SceneManager::StartUp(DisplayManager *displayManager)
{
    m_displayManager = displayManager;

    return 1;
}

void SceneManager::ShutDown()
{
    //***********************************************************
    //       Possibly do nothing until serialization is implemented.
    //***********************************************************
    m_scene.ShutDown();
}

void SceneManager::Update(float deltaTime, float physDeltaTime)
{
    m_scene.Update(deltaTime);
    m_scene.UpdatePhysics(physDeltaTime);

}


int PrimScene1::StartUp(DisplayManager* displayManager)
{
    //***********************************************************
    //       Add objects to Scene
    //***********************************************************

    ///////////////// Create Meshes and Materials ///////////
    SolidMaterial* blueMat = new SolidMaterial();
    blueMat->m_diffuse = glm::vec3(0.2f, 0.f, 0.9f);
    blueMat->m_roughness = 16;
    m_scene.AddMaterial("Solid:Blue", blueMat);
    SolidMaterial *whiteMat = new SolidMaterial();
    whiteMat->m_diffuse = glm::vec3(0.5f, 0.6f, 0.5f);
    m_scene.AddMaterial("Solid:While", whiteMat);
    TextureMaterial* crateTex = new TextureMaterial("../assets/textures/container2.png", 0);
    crateTex->m_roughness = 16;
    m_scene.AddMaterial("Tex:Crate", crateTex);

    Object* obj = new Object();
    obj->AddComponent(new Primitive(PrimitiveType::CUBE, crateTex));
    m_scene.AddObject(obj);






    /////////////////    Create camera    ///////////////////////
    Object *camera = new Object();
    camera->AddComponent(new Camera(displayManager));
    Camera *cam = camera->GetComponent<Camera>();
    cam->SetDirection(0, 0);
    cam->m_position = glm::vec3(0.5f, 0.5f, 1.5f);
    camera->AddComponent(new CameraController(*displayManager, *cam));
    m_scene.AddObject(camera);

    /////////////////    Create lights    ///////////////////////
    Object *dirLight = new Object();
    dirLight->AddComponent(new DirLight());
    dirLight->GetComponent<DirLight>()->m_direction = glm::vec3(-.5, -.75, -.3);
    m_scene.AddObject(dirLight);

    Object *ptLight = new Object();
    Transform& transform = ptLight->GetTransform();
    transform.scale = glm::vec3(.1f);
    ptLight->AddComponent(new PointLight(glm::vec3(1.f, .5f, 1.f)));
    ptLight->GetComponent<PointLight>()->m_specularIntensity = .3f;
    ptLight->GetComponent<PointLight>()->m_diffuseIntensity = .5f;
    ptLight->AddComponent(new Primitive(PrimitiveType::CUBE, blueMat));
    ptLight->AddComponent(new MovePtLight(*ptLight->GetComponent<PointLight>()));
    m_scene.AddObject(ptLight);


    //***********************************************************
    //       Add Systems to Scene
    //***********************************************************




    //***********************************************************
    //       MeshData systems
    //***********************************************************
//    ClothRod* clothRodSys = new ClothRod(cloth->GetComponent<RodCloth>()->m_gridMesh);
//    m_scene.AddPhysicsSystem(clothRodSys);

    m_scene.StartUp();

    return 1;

}


} // end namespace seng