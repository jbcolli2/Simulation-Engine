//
// Created by jcollin2 on 9/29/22.
//

#include "MeshCommon.h"

#include "Rendering/Mesh.h"

namespace seng
{


template<>
void SetVertexAttribs<Vert3x3x2f>()
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

};



glm::vec3 DynamicGridMesh::computeTriangleNormal(int vertIdx, int edge0VertIdx, int edge1VertIdx)
{
    int ii{-1}, jj{-1};
    Vector2MatrixIdx(vertIdx, m_Nx, ii, jj);
    int triIdx = 2*ii - 1 + 6*(jj-1);
    auto iter = m_cachedNormals.find(triIdx);
    if(iter != m_cachedNormals.end())
    {
        return iter->second;
    }

    glm::vec3 edge0{m_vertices[edge0VertIdx].x - m_vertices[vertIdx].x,
                    m_vertices[edge0VertIdx].y - m_vertices[vertIdx].y,
                    m_vertices[edge0VertIdx].z - m_vertices[vertIdx].z};
    glm::vec3 edge1{m_vertices[edge1VertIdx].x - m_vertices[vertIdx].x,
                    m_vertices[edge1VertIdx].y - m_vertices[vertIdx].y,
                    m_vertices[edge1VertIdx].z - m_vertices[vertIdx].z};
    m_cachedNormals[triIdx] = glm::cross(edge0, edge1);
    return m_cachedNormals[triIdx];
}



void DynamicGridMesh::computeNormals()
{
    m_cachedNormals.clear();
    glm::vec3 avgNormal{0.f};
    int vertIdx{-1};
    for (int ii = 1; ii < m_Nx - 1; ++ii) {
        for (int jj = 1; jj < m_Ny - 1; ++jj) {
            // Inner
            avgNormal.x = 0.f;
            avgNormal.y = 0.f;
            avgNormal.z = 0.f;
            vertIdx = mat2VecIdx(ii, jj);

            incrementNormalN(vertIdx, avgNormal);
            incrementNormalNW(vertIdx, avgNormal);
            incrementNormalNE(vertIdx, avgNormal);
            incrementNormalS(vertIdx, avgNormal);
            incrementNormalSE(vertIdx, avgNormal);
            incrementNormalSW(vertIdx, avgNormal);

            avgNormal /= 6.f;
            m_vertices[vertIdx].r = avgNormal.x;
            m_vertices[vertIdx].g = avgNormal.y;
            m_vertices[vertIdx].b = avgNormal.z;
        }

        // Top and bottom
        avgNormal.x = 0.f;
        avgNormal.y = 0.f;
        avgNormal.z = 0.f;
        vertIdx = mat2VecIdx(ii, 0);
        incrementNormalNW(vertIdx, avgNormal);
        incrementNormalN(vertIdx, avgNormal);
        incrementNormalNE(vertIdx, avgNormal);
        avgNormal /= 3.f;
        m_vertices[vertIdx].r = avgNormal.x;
        m_vertices[vertIdx].g = avgNormal.y;
        m_vertices[vertIdx].b = avgNormal.z;

        avgNormal.x = 0.f;
        avgNormal.y = 0.f;
        avgNormal.z = 0.f;
        vertIdx = mat2VecIdx(ii, m_Ny - 1);
        incrementNormalSW(vertIdx, avgNormal);
        incrementNormalS(vertIdx, avgNormal);
        incrementNormalSE(vertIdx, avgNormal);
        avgNormal /= 3.f;
        m_vertices[vertIdx].r = avgNormal.x;
        m_vertices[vertIdx].g = avgNormal.y;
        m_vertices[vertIdx].b = avgNormal.z;
    }

    // Left and Right
    for (int jj = 1; jj < m_Ny - 1; ++jj) {
        avgNormal.x = 0.f;
        avgNormal.y = 0.f;
        avgNormal.z = 0.f;
        vertIdx = mat2VecIdx(0, jj);
        incrementNormalNE(vertIdx, avgNormal);
        incrementNormalS(vertIdx, avgNormal);
        incrementNormalSE(vertIdx, avgNormal);
        avgNormal /= 3.f;
        m_vertices[vertIdx].r = avgNormal.x;
        m_vertices[vertIdx].g = avgNormal.y;
        m_vertices[vertIdx].b = avgNormal.z;

        avgNormal.x = 0.f;
        avgNormal.y = 0.f;
        avgNormal.z = 0.f;
        vertIdx = mat2VecIdx(m_Nx - 1, jj);
        incrementNormalSW(vertIdx, avgNormal);
        incrementNormalN(vertIdx, avgNormal);
        incrementNormalNW(vertIdx, avgNormal);
        avgNormal /= 3.f;
        m_vertices[vertIdx].r = avgNormal.x;
        m_vertices[vertIdx].g = avgNormal.y;
        m_vertices[vertIdx].b = avgNormal.z;
    }
}


void DynamicGridMesh::GenerateBuffers(MeshData& meshData)
{
    // Create element vector
    std::vector<unsigned int> elements(6*(m_Nx-1)*(m_Ny-1));

    int elementIdx = 0;
    for(int jj = 0; jj < m_Ny - 1; ++jj)
    {
        for(int ii = 0; ii < m_Nx - 1; ++ii)
        {
            elements[elementIdx] = ii + jj*m_Nx;
            elements[elementIdx+1] = ii + 1 + jj*m_Nx;
            elements[elementIdx+2] = ii + (jj+1)*m_Nx;

            elements[elementIdx+3] = ii + 1 + jj*m_Nx;
            elements[elementIdx+4] = ii + 1 + (jj+1)*m_Nx;
            elements[elementIdx+5] = ii + (jj+1)*m_Nx;

            elementIdx += 6;
        }
    }

    GenAndLoadVAO(meshData.m_vao, meshData.m_vao.m_vboID, meshData.m_vao.m_eboID, m_vertices, elements, GL_DYNAMIC_DRAW);
    meshData.m_numVertices = m_vertices.size();
    meshData.m_numIndices = elements.size();
}


void DynamicGridMesh::ReloadVBO(unsigned int vbo, const std::vector<Node>& nodes)
{
    for(int vertIdx = 0; vertIdx < nodes.size(); vertIdx++)
    {
        m_vertices[vertIdx].x = nodes[vertIdx].currentPosition.x;
        m_vertices[vertIdx].y = nodes[vertIdx].currentPosition.y;
        m_vertices[vertIdx].z = nodes[vertIdx].currentPosition.z;
    }

    computeNormals();

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size()*sizeof(Vert3x3x2f), &m_vertices[0], GL_DYNAMIC_DRAW);
}


VAO::VAO(VAO&& otherVAO) : m_vaoID(otherVAO.m_vaoID), m_vboID(otherVAO.m_vboID), m_eboID(otherVAO.m_eboID),
                           m_numVerts(otherVAO.m_numVerts), m_numElements(otherVAO.m_numElements)
{
    resetVAO();
}

VAO& VAO::operator=(VAO&& otherVAO)
{
    // Check for self assignment
    if(&otherVAO == this)
        return *this;

    otherVAO.m_vaoID = m_vaoID;
    otherVAO.m_vboID = m_vboID;
    otherVAO.m_eboID = m_eboID;
    otherVAO.m_numVerts = m_numVerts;
    otherVAO.m_numElements = m_numElements;

    resetVAO();
}

VAO::~VAO()
{
    glDeleteVertexArrays(1, &m_vaoID);
    glDeleteBuffers(1, &m_vboID);
    glDeleteBuffers(1, &m_eboID);
}


} // seng