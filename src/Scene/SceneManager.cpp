//
// Created by jcollin2 on 7/1/22.
//

#include "Misc/Common.h"
#include "../include/stb_image.h"

#include "Scene/SceneManager.h"
#include "Engine/Object.h"

#include "Rendering/Assets.h"

#include "Components/MeshComponents.h"
#include "Components/Lights.h"
#include "Components/Camera.h"
#include "Components/PhysicsComponents.h"


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



/***************** AddObject Factory Methods  ******************
 * @brief Create object on the heap and add it to the scene.
******************************************************************///
void SceneManager::AddEmptyObject(std::string id, const Transform& transform)
{
    auto object = std::make_unique<Object>(id);
    object->SetTransform(transform);
    m_scene.AddObject(std::move(object));
}

void SceneManager::AddCube(std::string id, std::string materialID, const Transform& transform)
{
    auto object = std::make_unique<Object>(id);
    object->SetTransform(transform);
    object->AddComponent(new Primitive(PrimitiveType::CUBE, m_scene.GetMaterial(materialID)));
    m_scene.AddObject(std::move(object));
}

void SceneManager::AddPlane(std::string id, std::string materialID, const Transform& transform)
{
    auto object = std::make_unique<Object>(id);
    object->SetTransform(transform);
    object->AddComponent(new Primitive(PrimitiveType::PLANE, m_scene.GetMaterial(materialID)));
    m_scene.AddObject(std::move(object));
}

void SceneManager::AddSphere(std::string id, std::string materialID, unsigned int subdivideIters, const Transform& transform)
{
    auto object = std::make_unique<Object>(id);
    object->SetTransform(transform);
    object->AddComponent(new Primitive(PrimitiveType::SPHERE, m_scene.GetMaterial(materialID)));
    m_scene.AddObject(std::move(object));
}


void SceneManager::AddModel(std::string id, std::string modelID, const Transform& transform)
{
    auto object = std::make_unique<Object>(id);
    object->SetTransform(transform);
    object->AddComponent(new Model(*m_scene.GetModel(modelID)));
    m_scene.AddObject(std::move(object));
}

void
SceneManager::AddPointLight(std::string id, glm::vec3 position, bool movable, float diffIntensity, float specIntensity,
                            glm::vec3 color)
{
    auto object = std::make_unique<Object>(id);
    object->AddComponent(new PointLight(position, color));
    object->GetComponent<PointLight>().m_diffuseIntensity = diffIntensity;
    object->GetComponent<PointLight>().m_specularIntensity = specIntensity;
    if(movable)
        object->AddComponent(new MovePtLight(object->GetComponent<PointLight>()));

    m_scene.AddObject(std::move(object));
}

void SceneManager::AddDirLight(std::string id, glm::vec3 direction, float diffIntensity, float specIntensity,
                               glm::vec3 color)
{
    auto object = std::make_unique<Object>(id);
    object->AddComponent(new DirLight(direction, color));
    object->GetComponent<DirLight>().m_diffuseIntensity = diffIntensity;
    object->GetComponent<DirLight>().m_specularIntensity = specIntensity;

    m_scene.AddObject(std::move(object));

}

void SceneManager::AddCamera(std::string id, glm::vec3 position, glm::vec3 direction, bool controllable)
{
    auto object = std::make_unique<Object>(id);
    object->AddComponent(new Camera(position, direction, *m_displayManager));
    if(controllable)
        object->AddComponent(new CameraController(*m_displayManager, object->GetComponent<Camera>()));

    m_scene.AddObject(std::move(object));
}









//***********************************************************
//       Primitive Scene
//***********************************************************
int PrimScene1::StartUp(DisplayManager* displayManager)
{
    SceneManager::StartUp(displayManager);

    //***********************************************************
    //       Load Assets
    //***********************************************************
    m_scene.LoadTexture("Tex:Crate", "../assets/textures/container2.png");
    m_scene.LoadTexture("Tex:Coord", "../assets/textures/CoordTex.jpeg", true);
//    m_scene.LoadModel("Mod:Backpack", "../assets/models/backpack/backpack.obj", true);
    m_scene.LoadModel("Mod:Glass", "../assets/models/cocktail glass/cocktail glass.obj");

    //***********************************************************
    //       Create New Materials and add to Scene
    //***********************************************************
    std::unique_ptr<SolidMaterial> blueMat{new SolidMaterial()};
    blueMat->m_diffuse = glm::vec3(0.2f, 0.f, 0.9f);
    blueMat->m_roughness = 16;
    m_scene.AddMaterial("Solid:Blue", std::move(blueMat));

    std::unique_ptr<SolidMaterial> whiteMat{new SolidMaterial()};
    whiteMat->m_diffuse = glm::vec3(.7f, 0.7f, 0.7f);
    m_scene.AddMaterial("Solid:White", std::move(whiteMat));

    std::unique_ptr<SolidMaterial> mat{new SolidMaterial()};
    mat->m_diffuse = glm::vec3(.4f, 0.23f, 0.1f);
    m_scene.AddMaterial("Solid:Brown", std::move(mat));






    //***********************************************************
    //       Renderable Objects
    //***********************************************************
    Transform trans;
    trans = Transform();
    trans.position = glm::vec3(0.f, -1.f, 0.f);
    trans.scale = glm::vec3(10.f, 0.f, 10.f);
    AddPlane("Floor", "Solid:Brown", trans);

    trans.position = glm::vec3(-.5f, -.5f, -1.f);
    trans.scale = glm::vec3(.3f);
    AddSphere("Sphere", "Tex:Coord", 4, trans);

    trans = Transform();
    trans.position = glm::vec3(1.8f, .1f, -.4f);
    trans.scale = glm::vec3(.1f);
    AddModel("Glass", "Mod:Glass", trans);












    /////////////////    Create camera    ///////////////////////
    AddCamera("MainCamera", glm::vec3(.5f, .5f, 2.5f));

    /////////////////    Create lights    ///////////////////////
    AddDirLight("Direction Light", glm::vec3(-.5f, -.75f, -.3f));

    AddPointLight("Point Light 1", glm::vec3(1.f, .5f, 1.f), true);
    m_scene.FindObjectByID("Point Light 1").AddComponent(new Primitive(PrimitiveType::CUBE,
                                                                       m_scene.GetMaterial("Solid:White")));
    trans = Transform();
    trans.scale = glm::vec3(.1f);
    m_scene.FindObjectByID("Point Light 1").SetTransform(trans);





    //***********************************************************
    //       Add Cloth
    //***********************************************************
    auto cloth = std::make_unique<Object>("Cloth");
    cloth->AddComponent(new RodCloth(10, 10, RodCloth::m_fixAtTopEnds, m_scene.GetMaterial("Tex:Coord"), 1.f, 20));
    m_scene.AddObject(std::move(cloth));

    m_scene.StartUp();

    return 1;

}


} // end namespace seng