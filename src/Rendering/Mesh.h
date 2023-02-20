//
// Created by Jeb Collins on 3/4/22.
//

#ifndef PECS_RENDERER_RENDERABLE_HPP
#define PECS_RENDERER_RENDERABLE_HPP

#include "Misc/MeshCommon.h"
#include "Engine/Component.h"

#include "Rendering/Shader.h"
#include "Rendering/Material.h"

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

namespace seng
{




class VAOManager
{
private:
    std::unordered_map<std::string, std::unique_ptr<VAO>> m_vaoList{};

    VAOManager() = default;
public:

   static VAOManager& GetInstance()
   {
       static VAOManager manager{};
       return manager;
   }


    /***************** CreateVAO  ******************
     * @brief Given a list of vertices and (possibly) elements, create a VAO object.
     *
     *      - Add it to the m_vaoList with the passed in key
     *
     * @returns Raw pointer to the VAO object.  Usually to be kept in the Mesh object.
    ******************************************************************///
    VAO* CreateVAO(std::string vaoKey, std::vector<Vert3x3x2f> vertList, std::vector<unsigned int> elementList = std::vector<unsigned int>());

    /***************** GetVAO  ******************
     * @brief Returns a raw pointer for the VAO object associated with the passed in key.
     *
     *      Used when the VAO outlasts the game objects, like in Primitive or when the mesh
     *   is being instanced.  The VAO can exist once but be used by multiple Meshes
     *
     * @returns Raw pointer to VAO with key
    ******************************************************************///
    VAO* GetVAO(std::string vaoKey);

    /***************** DestroyVAO  ******************
     * @brief Destroys the VAO object held here in the Manager.
     *
     *      - Allows for memory management if we want to free up VAOs before the engine quits.
     *    Without this all VAOs used in the run will exist until engine closes, as this outside
     *    class owns them all.
     *
     *      - Must be carefull as this can cause dangling pointers to the VAO here.  Must make sure that such
     *    pointers are about to be destoryed when this method is called.
    ******************************************************************///
    void DestroyVAO(std::string vaoKey);
};





/***************** STRUCT: MeshData  ******************
 * @brief Contains all the data needed to render with OpenGL.
 *
 *      This includes the VAO objects, as well as the vertex and element list.
 *      Also the Material used for rendering
******************************************************************///
struct MeshData
{
public:
    VAO* m_vao;
    std::vector<Vert3x3x2f> m_vertices;
    std::vector<unsigned int> m_elements;
    Material& m_material;









};









/***************** Mesh Component  ******************
 * @brief Contains vector of MeshData objects and model matrix.
******************************************************************///


class Mesh : public Component
{
private:
    std::vector<MeshData> m_meshes;

public:
    glm::mat4 m_model;


    Mesh() = default;

    /***************** AddMeshDataNewVAO  ******************
     * @brief Add a new MeshData struct to the Mesh.  This will help us keep an accurate count of how
     *      many MeshData structs are in this Mesh.
     *
     *      - This will create a new VAO and add it to the VAO manager.
    ******************************************************************///
    void AddMeshDataNewVAO(const std::vector<Vert3x3x2f>& vertexList, const std::vector<unsigned int>& elementList = std::vector<unsigned int>(),
            const Material& material = *Material::m_defaultMaterial, std::string vaoKey = std::string())
    {
        // If a vaoKey was not passed in, name it by the object name
        if(vaoKey.size() == 0)
            vaoKey = "VAO_" + this->parentObject->GetId() + "_" + std::to_string(m_meshes.size());
        VAO* newVAO = VAOManager::GetInstance().CreateVAO(vaoKey,vertexList, elementList);
        m_meshes.push_back(MeshData{newVAO, vertexList, elementList, const_cast<Material&>(material)});
    }




    /***************** AddMeshDataExistingVAO  ******************
     * @brief Add a new MeshData struct to the Mesh.
     *
     *      - This method is used when the vao already exists in the VAOManager and the key is known by the calling component
     *             i.e. Primitive has a list of existing vaoKeys for each primitive type
    ******************************************************************///
    void AddMeshDataExistingVAO(const std::string& vaoKey, const std::vector<Vert3x3x2f>& vertexList, const std::vector<unsigned int>& elementList = std::vector<unsigned int>(),
                           const Material& material = *Material::m_defaultMaterial)
    {
        VAO* newVAO = VAOManager::GetInstance().GetVAO(vaoKey);
        m_meshes.push_back(MeshData{newVAO, vertexList, elementList, const_cast<Material&>(material)});
    }

    
    /***************** GetSingleMeshData  ******************
     * @brief Returns a single MeshData struct at index.
     *
     * @param index Index into the m_meshes std::vector
     * 
     * @returns Reference to MeshData so it can be changed
    ******************************************************************///
    MeshData& GetSingleMeshData(int index)
    {
        assert(index < m_meshes.size() && "Mesh::GetSingleMeshData index out of bounds");
        return *m_meshes[index];
    }

    /***************** GetAllMeshData  ******************
     * @brief Return all the mesh data in the mesh
     *
     * @returns m_meshes
    ******************************************************************///
    std::vector<std::unique_ptr<MeshData>>& GetAllMeshData()
    {
        return m_meshes;
    }
    
    /***************** UpdateModelMatrix  ******************
     * @brief If the Transform has changed, update the model matrix with Transform data
    ******************************************************************///
    void UpdateModelMatrix()
    {
        Transform& transform = parentObject->GetTransform();
        if(transform.updateModelMatrix)
        {
            m_model = glm::translate(glm::mat4(1.f), transform.position);
            m_model = glm::rotate(m_model, glm::radians(transform.rotation.x), glm::vec3(1.f, 0.f, 0.f));
            m_model = glm::rotate(m_model, glm::radians(transform.rotation.y), glm::vec3(0.f, 1.f, 0.f));
            m_model = glm::rotate(m_model, glm::radians(transform.rotation.z), glm::vec3(0.f, 0.f, 1.f));
            m_model = glm::scale(m_model, transform.scale);
        }
    }



    void Render(Shader& shader);
};


} // End seng namespace
#endif //PECS_RENDERER_RENDERABLE_HPP
