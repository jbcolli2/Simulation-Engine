//
// Created by jcollin2 on 2/12/23.
//

#ifndef SIM_ENGINE_DYNAMICMESHCOMMON_H
#define SIM_ENGINE_DYNAMICMESHCOMMON_H

#include "Misc/Common.h"
#include "Misc/MeshCommon.h"

#include "Rendering/Mesh.h"


namespace seng
{

class DynamicGridMesh
{
private:
    int m_Nx{0}, m_Ny{0};                               // Number of vertices in x and y axes
    std::vector<Vert3x3x2f> m_vertices;                 //Rendering vertex list

    std::unordered_map<int, glm::vec3> m_cachedNormals{}; // Cache the normal of each triangle as it is computed




    /***************** computeNormals  ******************
     * @brief Computes the normals based on the position of each
     *      of the points on the grid.  Normals are computed as the average of
     *      the normals of all triangles that use that point.
    ******************************************************************///
    void computeNormals();

    /***************** computeTriangleNormal  ******************
     * @brief The grunt work for incrementNormal**.  Compute the normal of a given
     *      triangle defined by vertices vert, edge0Vert, and edge1Vert, which are
     *      passed in by the incrementNormal** methods.
     *
     * @param vertIdx Index into m_vertices of center vertex
     * @param edge0VertIdx Index of vertex to define edge0 of triangle
     * @param edge1VertIdx Index of vertex to define edge1 of triangle
     *
     * @returns Normal of the triangle
    ******************************************************************///
    glm::vec3 computeTriangleNormal(int vertIdx, int edge0VertIdx, int edge1VertIdx);

    /***************** incrementNormalS/SE/SW/...  ******************
     * @brief Compute the normal for one particular triangle, either S, SW, SE, N, NW, or NE.
     *
     *      Add that normal to incVector.
     *
     * @param vert Index of center vertex that we are averaging around
     * @param incVector Vector to increment by computed normal
     *
    ******************************************************************///
    void incrementNormalS(int vertIdx, glm::vec3& incVector)
    {
        incVector += computeTriangleNormal(vertIdx, vertIdx - m_Nx, vertIdx - m_Nx + 1);
    }

    void incrementNormalSE(int vertIdx, glm::vec3& incVector)
    {
        incVector += computeTriangleNormal(vertIdx, vertIdx - m_Nx + 1, vertIdx + 1);
    }

    void incrementNormalSW(int vertIdx, glm::vec3& incVector)
    {
        incVector += computeTriangleNormal(vertIdx, vertIdx - 1, vertIdx - m_Nx);
    }

    void incrementNormalNE(int vertIdx, glm::vec3& incVector)
    {
        incVector += computeTriangleNormal(vertIdx, vertIdx + 1, vertIdx + m_Nx);
    }

    void incrementNormalN(int vertIdx, glm::vec3& incVector)
    {
        incVector += computeTriangleNormal(vertIdx, vertIdx + m_Nx, vertIdx + m_Nx - 1);
    }

    void incrementNormalNW(int vertIdx, glm::vec3& incVector)
    {
        incVector += computeTriangleNormal(vertIdx, vertIdx + m_Nx - 1, vertIdx - 1);
    }

    /***************** mat2VecIdx  ******************
     * @brief Convert (ii,jj) matrix indices into vector index assuming matrix has m_Nx columns.
     *
     * @returns Vector index associated with (ii,jj) matrix indices.
    ******************************************************************///
    int mat2VecIdx(int ii, int jj)
    {
        return seng::Matrix2VectorIdx(ii, jj, m_Nx);
    };


public:
    DynamicGridMesh(int Nx, int Ny) : m_Nx(Nx), m_Ny(Ny), m_vertices{std::vector<Vert3x3x2f>(Nx * Ny)}
    {};

    /***************** UpdateVerticesAndReloadVBO  ******************
     * @brief As position of the vertices change, update m_vertices and then load the data
     *      into the passed vbo.
     *
     * @param vbo vertex buffer to update with new vertex data
     * @param nodes Data of the vertex nodes from a physical component like cloth.  Data will have currentPosition
     *              member to update position of the vertices.
    ******************************************************************///
    void UpdateVerticesAndReloadVBO(const std::vector<Node>& nodes, Mesh& mesh);


    int GetNumVertices()
    { return m_vertices.size(); };

    int GetNumElements()
    { return 6 * (m_Nx - 1) * (m_Ny - 1); };
};

} // Namespace seng

#endif //SIM_ENGINE_DYNAMICMESHCOMMON_H
