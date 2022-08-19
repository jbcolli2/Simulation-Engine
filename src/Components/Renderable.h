//
// Created by Jeb Collins on 3/4/22.
//

#ifndef PECS_RENDERER_RENDERABLE_HPP
#define PECS_RENDERER_RENDERABLE_HPP

#include "Scene/Mesh.h"
#include "Scene/PrimitiveMesh.h"
#include "Scene/GridMesh.h"
#include "Engine/Component.h"

#include "glm/glm.hpp"
#include <vector>

namespace seng
{


/***************** Primitive  ******************
 * @brief Component represents the object is a primitive shape.  Holds the
 *      mesh data for the shape.
******************************************************************///
class Cube : public Component
{
public:
    PrimitiveMesh* m_mesh{};

    Cube() : m_mesh(new PrimitiveMesh(PrimitiveType::CUBE)) {};
};


class Plane : public Component
{
public:
    PrimitiveMesh* m_mesh{};

    Plane() : m_mesh(new PrimitiveMesh(PrimitiveType::PLANE)) {};
};


class Sphere : public Component
{
public:
    PrimitiveMesh* m_mesh{};

    Sphere() : m_mesh(new PrimitiveMesh(PrimitiveType::SPHERE)) {};
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


/***************** Renderable Component  ******************
 * @brief Contains vector of Mesh objects and model matrix.
******************************************************************///


class Renderable : public Component
{
public:
    Renderable() = default;
    Renderable(Mesh* mesh)
    {
        m_meshes.push_back(mesh);
    }
    std::vector<Mesh*> m_meshes;
    glm::mat4 m_model;

    void Draw(Shader& shader)
    {
        for(Mesh* mesh : m_meshes)
        {
            mesh->SetupMaterial(shader);
            shader.setUniformMatrix4f("model", m_model);
            mesh->Draw(shader);
        }
    }

};


} // End seng namespace
#endif //PECS_RENDERER_RENDERABLE_HPP
