//
// Created by jcollin2 on 7/1/22.
//

#include "Misc/Common.h"

#include "Scene/SceneManager.h"
#include "Engine/Object.h"

#include "Scene/PrimitiveMesh.h"
#include "Scene/GridMesh.h"
#include "Scene/Material.h"

#include "Systems/CameraController.h"
#include "Systems/FixedGridMesh.h"
#include "Systems/ClothRod.h"


namespace seng
{


/***************** StartUp  ******************
     * @brief Load the scene with objects.
     *      TODO: This will be deserialized from JSON file in future.
******************************************************************///
int seng::SceneManager::StartUp(DisplayManager *displayManager)
{
    m_displayManager = displayManager;


    //***********************************************************
    //       Add objects to Scene
    //***********************************************************

    ///////////////// Create Meshes and Materials ///////////
    SolidMaterial *blueMat = new SolidMaterial();
    blueMat->m_diffuse = glm::vec3(0.2f, 0.f, 0.9f);
    blueMat->m_roughness = 16;
    SolidMaterial *whiteMat = new SolidMaterial();
    whiteMat->m_diffuse = glm::vec3(0.5f, 0.6f, 0.5f);


    PrimitiveMesh *cubeMesh = new PrimitiveMesh(PrimitiveType::CUBE);
    cubeMesh->SetMaterial(blueMat);
    PrimitiveMesh *lightCubeMesh = new PrimitiveMesh(PrimitiveType::CUBE);
    lightCubeMesh->SetMaterial(whiteMat);
    PrimitiveMesh *planeMesh = new PrimitiveMesh(PrimitiveType::PLANE);
    planeMesh->SetMaterial(whiteMat);

    GridMesh *clothMesh = new GridMesh(50, 50);
    clothMesh->SetMaterial(blueMat);




    ///////////////// Create renderables ///////////////////////
    Object *cube = new Object();
    cube->GetTransform().position = glm::vec3(0.f, 0.f, -2.f);
    cube->AddComponent(new Sphere());
    cube->GetComponent<Sphere>()->m_mesh->SetMaterial(blueMat);
    Renderable *tempRend = new Renderable(cube->GetComponent<Sphere>()->m_mesh);
    cube->AddComponent(tempRend);
    m_scene.AddObject(cube);

    Object *floor = new Object();
    floor->GetTransform().scale = glm::vec3(10.f, 1.f, 10.f);
//    floor->GetTransform().position.y = -2.f;
    floor->AddComponent(new Plane());
    floor->GetComponent<Plane>()->m_mesh->SetMaterial(whiteMat);
    floor->AddComponent(new Renderable(floor->GetComponent<Plane>()->m_mesh));
    m_scene.AddObject(floor);

    Object* cloth = new Object();
    cloth->AddComponent(new RodCloth(30,30));
    cloth->GetComponent<RodCloth>()->m_gridMesh->SetMaterial(blueMat);
    tempRend = new Renderable();
    tempRend->m_meshes.push_back(cloth->GetComponent<RodCloth>()->m_gridMesh);
    cloth->AddComponent(tempRend);
    m_scene.AddObject(cloth);



    /////////////////    Create camera    ///////////////////////
    Object *camera = new Object();
    camera->AddComponent(new Camera(displayManager));
    Camera *cam = camera->GetComponent<Camera>();
    cam->SetDirection(0, 0);
    cam->m_position = glm::vec3(0.5f, 0.5f, 1.5f);
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
    ptLight->AddComponent(new Renderable(lightCubeMesh));
    m_scene.AddObject(ptLight);


    //***********************************************************
    //       Add Systems to Scene
    //***********************************************************
    CameraController* cameraController = new CameraController(m_displayManager);
    cameraController->AddObject(camera);
    m_scene.AddSystem(cameraController);

    RenderableSystem* renderSystem = new RenderableSystem();
    std::vector<Object *>& sceneRenderables = m_scene.GetRenderables();
    for (Object *renderObject: sceneRenderables) {
        renderSystem->AddObject(renderObject);
    }
    m_scene.AddSystem(renderSystem);

    MovePtLight* movePtLightSystem = new MovePtLight();
    movePtLightSystem->AddObject(ptLight);
    m_scene.AddSystem(movePtLightSystem);



    //***********************************************************
    //       Mesh systems
    //***********************************************************
    ClothRod* clothRodSys = new ClothRod(cloth->GetComponent<RodCloth>()->m_gridMesh);
    m_scene.AddPhysicsSystem(clothRodSys);

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

void SceneManager::Update(float deltaTime, float physDeltaTime)
{
    m_scene.Update(deltaTime);
    m_scene.UpdatePhysics(physDeltaTime);

}


} // end namespace seng