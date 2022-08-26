//
// Created by jcollin2 on 7/29/22.
//

#include "GridMesh.h"


namespace seng
{







GridMesh::GridMesh(int N_x, int N_y) : m_Nx(N_x), m_Ny(N_y)
{
    m_vertices = std::vector<Vert3x3x2f>(N_x*N_y);
    // There are (Nx-1)*(Ny-1) boxes in the grid, and 6 verts per box
    m_elements = std::vector<unsigned int>((N_x-1)*(N_y-1)*6);
    fillElements();

    // Fill grid with flat plane [0,1] x {0} x [0,1]
    for(int ii = 0; ii < N_x; ++ii)
    {
        for(int jj = 0; jj < N_y; ++jj)
        {
           m_vertices[mat2VecIdx(ii, jj)].x = static_cast<float>(ii)/(N_x-1);
           m_vertices[mat2VecIdx(ii, jj)].y = 0.f;
           m_vertices[mat2VecIdx(ii, jj)].z = static_cast<float>(jj)/(N_y-1);
        }
    }

    SetupForDraw();

    m_numVertices = m_vertices.size();

    //******* VBO/VAO   ***************
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    loadDataToVBO(m_vbo, m_vertices, GL_DYNAMIC_DRAW);
    loadDataToEBO(m_ebo, m_elements, GL_DYNAMIC_DRAW);

    SetVertexAttribs<Vert3x3x2f>();

    glBindVertexArray(0);

//    for(int ii = 0; ii < N_x; ++ii)
//    {
//        for(int jj = 0; jj < N_y; ++jj)
//        {
//            std::cout << "Grid2Triangle("<<ii<<", "<<jj<<") = ";
//            for(int ll : m_gridToTriangle.index(ii,jj))
//                std::cout << ll << " | ";
//            std::cout << "\n";
//            std::cout << "Grid2Vertex("<<ii<<", "<<jj<<") = ";
//            for(int ll : m_gridToTriVertex.index(ii,jj))
//                std::cout << ll << " | ";
//            std::cout << "\n";
//        }
//    }

}









/***************** SetupForDraw  ******************
 * @brief Setup the vertex data after changing the grid positions.  Also calculate the normals.
******************************************************************///
void GridMesh::SetupForDraw()
{
    m_cachedNormals.clear();
    computeNormals();

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size()*sizeof(Vert3x3x2f), &m_vertices[0], GL_DYNAMIC_DRAW);
}



void GridMesh::Draw(Shader& shader)
{
    if(m_positionChanged)
    {
        SetupForDraw();
        m_positionChanged = false;
    }

    glBindVertexArray(m_vao);
//    glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
    glDrawElements(GL_TRIANGLES, m_elements.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


void GridMesh::computeNormals()
{
    glm::vec3 avgNormal{0.f};
    int vert{-1};
    for(int ii = 1; ii < m_Nx - 1; ++ii)
    {
        for(int jj = 1; jj < m_Ny - 1; ++jj)
        {
            // Inner
            avgNormal.x = 0.f; avgNormal.y = 0.f; avgNormal.z = 0.f;
            vert = mat2VecIdx(ii, jj);

            incrementNormalN(vert, avgNormal);
            incrementNormalNW(vert, avgNormal);
            incrementNormalNE(vert, avgNormal);
            incrementNormalS(vert, avgNormal);
            incrementNormalSE(vert, avgNormal);
            incrementNormalSW(vert, avgNormal);

            avgNormal /= 6.f;
            m_vertices[vert].r = avgNormal.x;
            m_vertices[vert].g = avgNormal.y;
            m_vertices[vert].b = avgNormal.z;
        }

        // Top and bottom
        avgNormal.x = 0.f; avgNormal.y = 0.f; avgNormal.z = 0.f;
        vert = mat2VecIdx(ii, 0);
        incrementNormalNW(vert, avgNormal);
        incrementNormalN(vert, avgNormal);
        incrementNormalNE(vert, avgNormal);
        avgNormal /= 3.f;
        m_vertices[vert].r = avgNormal.x;
        m_vertices[vert].g = avgNormal.y;
        m_vertices[vert].b = avgNormal.z;

        avgNormal.x = 0.f; avgNormal.y = 0.f; avgNormal.z = 0.f;
        vert = mat2VecIdx(ii, m_Ny-1);
        incrementNormalSW(vert, avgNormal);
        incrementNormalS(vert, avgNormal);
        incrementNormalSE(vert, avgNormal);
        avgNormal /= 3.f;
        m_vertices[vert].r = avgNormal.x;
        m_vertices[vert].g = avgNormal.y;
        m_vertices[vert].b = avgNormal.z;
    }

    // Left and Right
    for(int jj = 1; jj < m_Ny-1; ++jj)
    {
        avgNormal.x = 0.f; avgNormal.y = 0.f; avgNormal.z = 0.f;
        vert = mat2VecIdx(0, jj);
        incrementNormalNE(vert, avgNormal);
        incrementNormalS(vert, avgNormal);
        incrementNormalSE(vert, avgNormal);
        avgNormal /= 3.f;
        m_vertices[vert].r = avgNormal.x;
        m_vertices[vert].g = avgNormal.y;
        m_vertices[vert].b = avgNormal.z;

        avgNormal.x = 0.f; avgNormal.y = 0.f; avgNormal.z = 0.f;
        vert = mat2VecIdx(m_Nx-1, jj);
        incrementNormalSW(vert, avgNormal);
        incrementNormalN(vert, avgNormal);
        incrementNormalNW(vert, avgNormal);
        avgNormal /= 3.f;
        m_vertices[vert].r = avgNormal.x;
        m_vertices[vert].g = avgNormal.y;
        m_vertices[vert].b = avgNormal.z;
    }


    // Corners
    avgNormal.x = 0.f; avgNormal.y = 0.f; avgNormal.z = 0.f;
    vert = 0;
    incrementNormalNE(vert, avgNormal);
    m_vertices[vert].r = avgNormal.x;
    m_vertices[vert].g = avgNormal.y;
    m_vertices[vert].b = avgNormal.z;

    avgNormal.x = 0.f; avgNormal.y = 0.f; avgNormal.z = 0.f;
    vert = m_Nx-1;
    incrementNormalNW(vert, avgNormal);
    incrementNormalN(vert, avgNormal);
    avgNormal /= 2.f;
    m_vertices[vert].r = avgNormal.x;
    m_vertices[vert].g = avgNormal.y;
    m_vertices[vert].b = avgNormal.z;

    avgNormal.x = 0.f; avgNormal.y = 0.f; avgNormal.z = 0.f;
    vert = m_Nx*(m_Ny-1);
    incrementNormalS(vert, avgNormal);
    incrementNormalSE(vert, avgNormal);
    avgNormal /= 2.f;
    m_vertices[vert].r = avgNormal.x;
    m_vertices[vert].g = avgNormal.y;
    m_vertices[vert].b = avgNormal.z;

    avgNormal.x = 0.f; avgNormal.y = 0.f; avgNormal.z = 0.f;
    vert = m_Nx*m_Ny - 1;
    incrementNormalSW(vert, avgNormal);
    m_vertices[vert].r = avgNormal.x;
    m_vertices[vert].g = avgNormal.y;
    m_vertices[vert].b = avgNormal.z;
}




/***************** fillElements  ******************
 * @brief Fill the elements vector based on the Nx and Ny of the grid.
******************************************************************///
void GridMesh::fillElements()
{
    int elementIdx = 0;
    for(int jj = 0; jj < m_Ny - 1; ++jj)
    {
        for(int ii = 0; ii < m_Nx - 1; ++ii)
        {
            m_elements[elementIdx] = ii + jj*m_Nx;
            m_elements[elementIdx+1] = ii + 1 + jj*m_Nx;
            m_elements[elementIdx+2] = ii + (jj+1)*m_Nx;

            m_elements[elementIdx+3] = ii + 1 + jj*m_Nx;
            m_elements[elementIdx+4] = ii + 1 + (jj+1)*m_Nx;
            m_elements[elementIdx+5] = ii + (jj+1)*m_Nx;

            elementIdx += 6;
        }
    }
}


} // end namespace seng