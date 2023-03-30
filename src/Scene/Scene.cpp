//
// Created by jcollin2 on 7/19/22.
//

#include "Scene/Scene.h"
#include "../include/stb_image.h"

#include "Engine/Object.h"
#include "Systems/System.h"

#include "Components/Lights.h"
#include "Components/Camera.h"

#include "Rendering/Mesh.h"
#include "Rendering/Assets.h"

namespace seng
{


Scene::Scene()
{
    m_materialList = std::unordered_map<std::string, std::unique_ptr<Material>>();
    m_objects = std::vector<std::unique_ptr<Object>>();
}



/***************** StartUp  ******************
 * @brief Call startup on all systems.
******************************************************************///
void Scene::StartUp()
{
    m_ambientIntensity = .2f;
    for (auto& object: m_objects) {
        object->StartUp();
    }
}




/***************** Update  ******************
 * @brief Update all objects in the scene.  Mostly done through
 *      calling update on all systems in the scene.
 *
 * @param deltaTime delta from last frame
******************************************************************///
void Scene::Update(float deltaTime)
{
    for (auto& object: m_objects) {
        object->Update(deltaTime);
    }
}


void Scene::UpdatePhysics(float deltaTime)
{
    m_physTimer += deltaTime;

    while (m_physTimer > m_physTimeStep) {
        for (auto& object: m_objects) {
            object->FixedUpdate(m_physTimeStep);
        }
//        for(System* system : m_physSystems)
//        {
//            system->Update(m_physTimeStep);
//        }

        m_physTimer -= m_physTimeStep;
    }
}




//***********************************************************
//       Load Assets
//***********************************************************
void Scene::LoadTexture(const std::string& textureID, const std::string& path, bool flip)
{
    if(flip)
        stbi_set_flip_vertically_on_load(flip);

    AddMaterial(textureID, new TextureMaterial(path, 0));

    if(flip)
        stbi_set_flip_vertically_on_load(false);
}




void Scene::LoadModel(const std::string& modelID, const std::string& path, bool flip)
{
    if(flip)
        stbi_set_flip_vertically_on_load(flip);

    AddModelAsset(modelID, new ModelAsset(path));

    if(flip)
        stbi_set_flip_vertically_on_load(false);
}


//***********************************************************
//       Add things to the scene
//***********************************************************
void Scene::AddObject(std::unique_ptr<Object>&& object)
{
    std::string idName = object->GetId();
    assert(m_objectID.find(idName) == m_objectID.end() && "Already have object with that id.");
    m_objects.push_back(std::move(object));
    unsigned int objectIndex = m_objects.size() - 1;
    m_objectID[idName] = objectIndex;
    Object& tempObject = *m_objects[objectIndex];

    if (tempObject.HasComponent<Camera>()) {
        m_cameras.push_back(objectIndex);
    }
    if (tempObject.HasComponent<DirLight>() || tempObject.HasComponent<PointLight>()) {
        m_lights.push_back(objectIndex);
    }
}

void Scene::AddMaterial(const std::string& name, std::unique_ptr<Material>&& material)
{
    m_materialList[name] = std::move(material);
}

void Scene::AddMaterial(const std::string& name, Material*&& material)
{
    AddMaterial(name, std::unique_ptr<Material>(material));
}

void Scene::AddModelAsset(const std::string& name, std::unique_ptr<ModelAsset> model)
{
    m_modelList[name] = std::move(model);
}

void Scene::AddModelAsset(const std::string& name, ModelAsset*&& model)
{
    AddModelAsset(name, std::unique_ptr<ModelAsset>(model));
}









/***************** GetMainCamera  ******************
 * @brief Return a pointer to the main camera in the scene.
 *
 *      TODO: Update this to return a particular camera, not just
 *          the first one in the vector.
 *
 * @returns Main camera
******************************************************************///
Camera& Scene::GetMainCamera()
{
    return m_objects[m_cameras[0]]->GetComponent<Camera>();
}


Material* Scene::GetMaterial(std::string id)
{
    try
    {
        return m_materialList.at(id).get();
    }
    catch(std::out_of_range)
    {
        return Material::m_defaultMaterial.get();
    }
}

ModelAsset* Scene::GetModel(const std::string& id)
{
    try
    {
        return m_modelList.at(id).get();
    }
    catch (std::out_of_range)
    {
        assert("Scene::GetModel:  Model ID not present in Scene");
        return nullptr;
    }
}

std::vector<Material*> Scene::GetAllMaterials()
{
    std::vector<Material*> materialList;
    for(auto& material : m_materialList)
    {
        materialList.push_back(material.second.get());
    }

    return materialList;
}

std::vector<ModelAsset*> Scene::GetAllModels()
{
    std::vector<ModelAsset*> modelList;
    for(auto& model : m_modelList)
    {
        modelList.push_back(model.second.get());
    }

    return modelList;
}

Object& Scene::FindObjectByID(std::string id)
{
    return *m_objects[m_objectID[id]];
}


} // end namespace seng