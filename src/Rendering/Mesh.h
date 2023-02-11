//
// Created by Jeb Collins on 3/4/22.
//

#ifndef PECS_RENDERER_RENDERABLE_HPP
#define PECS_RENDERER_RENDERABLE_HPP

#include "Misc/MeshCommon.h"
#include "Engine/Component.h"

#include "Rendering/Shader.h"

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

namespace seng
{


class Material;
class MeshData
{
public:
    VAO m_vao;

    Material* m_material{};

    // Ctor
    template<typename VertT, typename ElementT>
    MeshData(std::vector<VertT> vertexList, std::vector<ElementT> elementList) : m_vao(vertexList, elementList)
    {};


    void Render(Shader& shader);


};









/***************** Mesh Component  ******************
 * @brief Contains vector of MeshData objects and model matrix.
******************************************************************///


class Mesh : public Component
{
public:
    Mesh() = default;
    std::vector<std::unique_ptr<MeshData>> m_meshes;
    glm::mat4 m_model;

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
};


} // End seng namespace
#endif //PECS_RENDERER_RENDERABLE_HPP
