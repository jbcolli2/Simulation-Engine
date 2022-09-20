//
// Created by Jeb Collins on 3/4/22.
//

#ifndef PECS_RENDERER_RENDERABLE_HPP
#define PECS_RENDERER_RENDERABLE_HPP

#include "Scene/PrimitiveMesh.h"
#include "Scene/GridMesh.h"
#include "Engine/Component.h"

#include "glm/glm.hpp"
#include <vector>

namespace seng
{


class Material;
class MeshData
{
public:
    unsigned int m_vao{0};
    unsigned int m_vbo{0};
    unsigned int m_ebo{0};
    unsigned int m_numVertices{0};
    unsigned int m_numIndices{0};

    Material* m_material{};

    // Ctor
    MeshData() = default;

};








class GridMesh;
/***************** RodCloth  ******************
 * @brief Holds data for cloth simulation using rods.
******************************************************************///
class RodCloth : public Component
{
public:
    GridMesh* m_gridMesh{nullptr};
    int m_numNodesX{-1}, m_numNodesY{-1};
    float m_mass{1.f};

    RodCloth(int Nx, int Ny)
    {
        m_gridMesh = new GridMesh(Nx, Ny);
        m_numNodesX = Nx;
        m_numNodesY = Ny;
    }
};


/***************** Mesh Component  ******************
 * @brief Contains vector of MeshData objects and model matrix.
******************************************************************///


class Mesh : public Component
{
public:
    Mesh() = default;
    std::vector<MeshData*> m_meshes;
    glm::mat4 m_model;
};


} // End seng namespace
#endif //PECS_RENDERER_RENDERABLE_HPP
