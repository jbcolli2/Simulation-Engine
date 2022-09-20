//
// Created by jcollin2 on 7/29/22.
//

#ifndef SIM_ENGINE_GRIDMESH_H
#define SIM_ENGINE_GRIDMESH_H

#include "Misc/Common.h"

#include "Rendering/MeshData.h"

namespace seng
{








class Shader;

class GridMesh : public MeshData
{
private:
    /***************** fillElements  ******************
     * @brief Fill the elements vector based on the Nx and Ny of the grid.
    ******************************************************************///
    void fillElements();




    int m_Nx{0}, m_Ny{0};
    std::vector<Vert3x3x2f> m_vertices;  //Rendering vertex list
    std::vector<unsigned int> m_elements; // Rendering element list

    std::unordered_map<int, glm::vec3> m_cachedNormals{}; // Cache the normal of each triangle as it is computed
    bool m_positionChanged{false};



    /***************** computeNormals  ******************
     * @brief Computes the normals based on the position of each
     *      of the points on the grid.  Normals are computed as the average of
     *      the normals of all triangles that use that point.
    ******************************************************************///
    void computeNormals();

    /***************** incrementNormalS/SE/SW/...  ******************
     * @brief Compute the normal for one particular triangle, either S, SW, SE, N, NW, or NE.
     *
     * @param vert Center vertex that we are averaging around
     * @param incVector Vector to increment by computed normal
     *
    ******************************************************************///
    void incrementNormalS(int vert, glm::vec3& incVector)
    {
        assert("There does not exists a S triangle for this node" && vert > m_Nx-1 && vert % (m_Nx) != m_Nx-1);

        int ii{-1}, jj{-1};
        Vector2MatrixIdx(vert, m_Nx, ii, jj);
        int triIdx = 2*ii + 6*(jj-1);
        auto iter = m_cachedNormals.find(triIdx);
        if(iter != m_cachedNormals.end())
        {
            incVector += iter->second;
            return;
        }

        glm::vec3 edge0{m_vertices[vert-m_Nx].x - m_vertices[vert].x,
                        m_vertices[vert-m_Nx].y - m_vertices[vert].y,
                        m_vertices[vert-m_Nx].z - m_vertices[vert].z};
        glm::vec3 edge1{m_vertices[vert-m_Nx+1].x - m_vertices[vert].x,
                        m_vertices[vert-m_Nx+1].y - m_vertices[vert].y,
                        m_vertices[vert-m_Nx+1].z - m_vertices[vert].z};
        m_cachedNormals[triIdx] = glm::cross(edge0, edge1);
        incVector += m_cachedNormals[triIdx];
    };
    void incrementNormalSE(int vert, glm::vec3& incVector)
    {
        assert("There does not exists a SE triangle for this node" && vert > m_Nx-1 && vert % (m_Nx) != m_Nx-1);

        int ii{-1}, jj{-1};
        Vector2MatrixIdx(vert, m_Nx, ii, jj);
        int triIdx = 2*ii + 1 + 6*(jj-1);
        auto iter = m_cachedNormals.find(triIdx);
        if(iter != m_cachedNormals.end())
        {
            incVector += iter->second;
            return;
        }

        glm::vec3 edge0{m_vertices[vert-m_Nx+1].x - m_vertices[vert].x,
                        m_vertices[vert-m_Nx+1].y - m_vertices[vert].y,
                        m_vertices[vert-m_Nx+1].z - m_vertices[vert].z};
        glm::vec3 edge1{m_vertices[vert+1].x - m_vertices[vert].x,
                        m_vertices[vert+1].y - m_vertices[vert].y,
                        m_vertices[vert+1].z - m_vertices[vert].z};
        m_cachedNormals[triIdx] = glm::cross(edge0, edge1);
        incVector += m_cachedNormals[triIdx];
    };
    void incrementNormalSW(int vert, glm::vec3& incVector)
    {
        assert("There does not exists a SW triangle for this node" && vert > m_Nx - 1 && vert % m_Nx != 0);

        int ii{-1}, jj{-1};
        Vector2MatrixIdx(vert, m_Nx, ii, jj);
        int triIdx = 2*ii - 1 + 6*(jj-1);
        auto iter = m_cachedNormals.find(triIdx);
        if(iter != m_cachedNormals.end())
        {
            incVector += iter->second;
            return;
        }

        glm::vec3 edge0{m_vertices[vert-1].x - m_vertices[vert].x,
                        m_vertices[vert-1].y - m_vertices[vert].y,
                        m_vertices[vert-1].z - m_vertices[vert].z};
        glm::vec3 edge1{m_vertices[vert-m_Nx].x - m_vertices[vert].x,
                        m_vertices[vert-m_Nx].y - m_vertices[vert].y,
                        m_vertices[vert-m_Nx].z - m_vertices[vert].z};
        m_cachedNormals[triIdx] = glm::cross(edge0, edge1);
        incVector += m_cachedNormals[triIdx];
    };
    void incrementNormalN(int vert, glm::vec3& incVector)
    {
        assert("There does not exists a N triangle for this node" && vert < m_Nx*(m_Ny - 1)  && vert % m_Nx != 0);

        int ii{-1}, jj{-1};
        Vector2MatrixIdx(vert, m_Nx, ii, jj);
        int triIdx = 2*ii - 1 + 6*jj;
        auto iter = m_cachedNormals.find(triIdx);
        if(iter != m_cachedNormals.end())
        {
            incVector += iter->second;
            return;
        }

        glm::vec3 edge0{m_vertices[vert+m_Nx].x - m_vertices[vert].x,
                        m_vertices[vert+m_Nx].y - m_vertices[vert].y,
                        m_vertices[vert+m_Nx].z - m_vertices[vert].z};
        glm::vec3 edge1{m_vertices[vert+m_Nx-1].x - m_vertices[vert].x,
                        m_vertices[vert+m_Nx-1].y - m_vertices[vert].y,
                        m_vertices[vert+m_Nx-1].z - m_vertices[vert].z};
        m_cachedNormals[triIdx] = glm::cross(edge0, edge1);
        incVector += m_cachedNormals[triIdx];
    };
    void incrementNormalNE(int vert, glm::vec3& incVector)
    {
        assert("There does not exists a NE triangle for this node" && vert < m_Nx*(m_Ny - 1)  && vert % (m_Nx) != m_Nx-1);

        int ii{-1}, jj{-1};
        Vector2MatrixIdx(vert, m_Nx, ii, jj);
        int triIdx = 2*ii + 6*jj;
        auto iter = m_cachedNormals.find(triIdx);
        if(iter != m_cachedNormals.end())
        {
            incVector += iter->second;
            return;
        }

        glm::vec3 edge0{m_vertices[vert+1].x - m_vertices[vert].x,
                        m_vertices[vert+1].y - m_vertices[vert].y,
                        m_vertices[vert+1].z - m_vertices[vert].z};
        glm::vec3 edge1{m_vertices[vert+m_Nx].x - m_vertices[vert].x,
                        m_vertices[vert+m_Nx].y - m_vertices[vert].y,
                        m_vertices[vert+m_Nx].z - m_vertices[vert].z};
        m_cachedNormals[triIdx] = glm::cross(edge0, edge1);
        incVector += m_cachedNormals[triIdx];
    };
    void incrementNormalNW(int vert, glm::vec3& incVector)
    {
        assert("There does not exists a NW triangle for this node" && vert < m_Nx*(m_Ny - 1)  && vert % m_Nx != 0);

        int ii{-1}, jj{-1};
        Vector2MatrixIdx(vert, m_Nx, ii, jj);
        int triIdx = 2*ii - 2 + 6*jj;
        auto iter = m_cachedNormals.find(triIdx);
        if(iter != m_cachedNormals.end())
        {
            incVector += iter->second;
            return;
        }

        glm::vec3 edge0{m_vertices[vert+m_Nx-1].x - m_vertices[vert].x,
                        m_vertices[vert+m_Nx-1].y - m_vertices[vert].y,
                        m_vertices[vert+m_Nx-1].z - m_vertices[vert].z};
        glm::vec3 edge1{m_vertices[vert-1].x - m_vertices[vert].x,
                        m_vertices[vert-1].y - m_vertices[vert].y,
                        m_vertices[vert-1].z - m_vertices[vert].z};
        m_cachedNormals[triIdx] = glm::cross(edge0, edge1);
        incVector += m_cachedNormals[triIdx];
    };

    /***************** mat2VecIdx  ******************
     * @brief Convert (ii,jj) matrix indices into vector index assuming matrix has m_Nx columns.
     *
     * @returns Vector index associated with (ii,jj) matrix indices.
    ******************************************************************///
    int mat2VecIdx(int ii, int jj)
    {
        return Matrix2VectorIdx(ii, jj, m_Nx);
    };

public:
    GridMesh(int N_x = 10, int N_y = 10);

    /***************** SetupForDraw  ******************
     * @brief Setup the vertex data after changeing the grid positions.  Also calculate the normals.
    ******************************************************************///
    void SetupForDraw();





    //***********************************************************
    //       Getter/Setter
    //***********************************************************
    int GetNx() {return m_Nx;};
    int GetNy() {return m_Ny;};

    void SetVertexPos(int idx, const glm::vec3& position)
    {
        m_vertices[idx].x = position.x;
        m_vertices[idx].y = position.y;
        m_vertices[idx].z = position.z;
        m_positionChanged = true;
    }
};

} // end namespace seng


#endif //SIM_ENGINE_GRIDMESH_H
