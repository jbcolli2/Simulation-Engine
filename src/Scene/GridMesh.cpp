//
// Created by jcollin2 on 7/29/22.
//

#include "GridMesh.h"


namespace seng
{







GridMesh::GridMesh(int N_x, int N_y) : m_Nx(N_x), m_Ny(N_y)
{
    m_gridPos = Grid<glm::vec3>(N_x, N_y);
    m_gridToTriangle = Grid<std::vector<int>>(N_x, N_y);
    m_gridToTriVertex = Grid<std::vector<int>>(N_x, N_y);
    m_vertices = std::vector<Vert3x3x2f>((N_x-1)*(N_y-1) * 6);

    // Fill grid with flat plane [0,1] x {0} x [0,1]
    for(int ii = 0; ii < N_x; ++ii)
    {
        for(int jj = 0; jj < N_y; ++jj)
        {
            m_gridPos.index(ii,jj) = glm::vec3(static_cast<float>(ii)/(N_x-1), 0.f, static_cast<float>(jj)/(N_y-1));
        }
    }

    setupGridToVertices();
    SetupForDraw();

    m_numVertices = m_vertices.size();

    //******* VBO/VAO   ***************
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    m_vbo = loadDataToVBO(m_vertices);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2 );
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


/***************** SetupGridToVertices  ******************
 * @brief Creates the data structures that allow conversion from the grid
 *      points to the vertices used for rendering.
 *
 *      One data structure `m_gridToTriangle` gives the index of the first vertex of all
 *      triangles associated with a point.
 *
 *      Other data structure `m_gridToTriVertex` gives which vertex of the triangle is associated
 *      with the grid point.
******************************************************************///
void GridMesh::setupGridToVertices()
{
    int tri;

    /////////////////    Fully interior points    ///////////////////////
    for(int ii = 1; ii < m_Nx-1; ++ii)
    {
        for(int jj = 1; jj < m_Ny-1; ++jj)
        {
            // Compute 3 South triangles
            addSWTriangle(ii, jj);
            addSTriangle(ii, jj);
            addSETriangle(ii, jj);
            addNWTriangle(ii, jj);
            addNTriangle(ii, jj);
            addNETriangle(ii, jj);
        }
    }

    /////////////////    Interior bottom and top points    ///////////////////////
    for(int ii = 1; ii < m_Nx-1; ++ii)
    {
        // Bottom points
        addNWTriangle(ii, 0);
        addNTriangle(ii, 0);
        addNETriangle(ii, 0);
        // Top points
        addSWTriangle(ii, m_Ny-1);
        addSTriangle(ii, m_Ny-1);
        addSETriangle(ii, m_Ny-1);
    }

    /////////////////    Interior left and right points    ///////////////////////
    for(int jj = 1; jj < m_Ny-1; ++jj)
    {
        // Left points
        addSTriangle(0, jj);
        addSETriangle(0, jj);
        addNETriangle(0, jj);
        // Right points
        addSWTriangle(m_Nx-1, jj);
        addNWTriangle(m_Nx-1, jj);
        addNTriangle(m_Nx-1, jj);
    }

    /////////////////    Corner points    ///////////////////////
    addNETriangle(0,0);  // Bottom left

    addNWTriangle(m_Nx-1, 0); // Bottom right
    addNTriangle(m_Nx-1, 0);

    addSTriangle(0, m_Ny-1); // Top left
    addSETriangle(0, m_Ny-1);

    addSWTriangle(m_Nx-1, m_Ny-1); // Top right
}



/***************** computeNormals  ******************
 * @brief Computes the normals based on the position of each
 *      of the points on the grid.  Normals are computed as the average of
 *      the normals of all triangles that use that point.
******************************************************************///
void GridMesh::computeNormals()
{
    glm::vec3 avgNormal;
    glm::vec3 edge01, edge02;
    for(int ii = 0; ii < m_Nx; ++ii)
    {
        for(int jj = 0; jj < m_Ny; ++jj)
        {
            avgNormal.x = 0.f;
            avgNormal.y = 0.f;
            avgNormal.z = 0.f;
            for(int tri : m_gridToTriangle.index(ii, jj))
            {
                edge01.x = m_vertices[tri+1].x - m_vertices[tri].x;
                edge01.y = m_vertices[tri+1].y - m_vertices[tri].y;
                edge01.z = m_vertices[tri+1].z - m_vertices[tri].z;

                edge02.x = m_vertices[tri+2].x - m_vertices[tri].x;
                edge02.y = m_vertices[tri+2].y - m_vertices[tri].y;
                edge02.z = m_vertices[tri+2].z - m_vertices[tri].z;

                avgNormal += glm::cross(edge01, edge02);
            }
            avgNormal /= m_gridToTriangle.index(ii,jj).size();

            for(int ll = 0; ll < m_gridToTriangle.index(ii,jj).size(); ++ll)
            {
                m_vertices[m_gridToTriangle.index(ii,jj)[ll] + m_gridToTriVertex.index(ii,jj)[ll]].r = avgNormal.x;
                m_vertices[m_gridToTriangle.index(ii,jj)[ll] + m_gridToTriVertex.index(ii,jj)[ll]].g = avgNormal.y;
                m_vertices[m_gridToTriangle.index(ii,jj)[ll] + m_gridToTriVertex.index(ii,jj)[ll]].b = avgNormal.z;
            }
        }
    }
}




/***************** SetupForDraw  ******************
 * @brief Setup the vertex data after changing the grid positions.  Also calculate the normals.
******************************************************************///
void GridMesh::SetupForDraw()
{
    for(int ii = 0; ii < m_Nx; ++ii)
    {
        for(int jj = 0; jj < m_Ny; ++jj)
        {
            for(int ll = 0; ll < m_gridToTriangle.index(ii,jj).size(); ++ll)
            {
                m_vertices[m_gridToTriangle.index(ii,jj)[ll] + m_gridToTriVertex.index(ii,jj)[ll]].x = m_gridPos.index(ii,jj).x;
                m_vertices[m_gridToTriangle.index(ii,jj)[ll] + m_gridToTriVertex.index(ii,jj)[ll]].y = m_gridPos.index(ii,jj).y;
                m_vertices[m_gridToTriangle.index(ii,jj)[ll] + m_gridToTriVertex.index(ii,jj)[ll]].z = m_gridPos.index(ii,jj).z;
            }
        }
    }

    computeNormals();

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size()*sizeof(Vert3x3x2f), &m_vertices[0], GL_STATIC_DRAW);

}



void GridMesh::Draw(Shader& shader)
{
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
}


//***********************************************************
//       Helpers to add triangle data to Grid2Vertex data structures
//***********************************************************
void GridMesh::addSWTriangle(int ii, int jj)
{
    m_gridToTriangle.index(ii,jj).push_back(3*(2*(jj-1)*(m_Nx-1) + (2*ii - 1)));
    m_gridToTriVertex.index(ii,jj).push_back(SW);
}
void GridMesh::addSTriangle(int ii, int jj)
{
    m_gridToTriangle.index(ii,jj).push_back(3*(2*(jj-1)*(m_Nx-1) + (2*ii)));
    m_gridToTriVertex.index(ii,jj).push_back(S);
}
void GridMesh::addSETriangle(int ii, int jj)
{
    m_gridToTriangle.index(ii,jj).push_back(3*(2*(jj-1)*(m_Nx-1) + (2*ii + 1)));
    m_gridToTriVertex.index(ii,jj).push_back(SE);
}
void GridMesh::addNWTriangle(int ii, int jj)
{
    m_gridToTriangle.index(ii,jj).push_back(3*(2*(jj)*(m_Nx-1) + (2*ii - 2)));
    m_gridToTriVertex.index(ii,jj).push_back(NW);
}
void GridMesh::addNTriangle(int ii, int jj)
{
    m_gridToTriangle.index(ii,jj).push_back(3*(2*(jj)*(m_Nx-1) + (2*ii - 1)));
    m_gridToTriVertex.index(ii,jj).push_back(N);
}
void GridMesh::addNETriangle(int ii, int jj)
{
    m_gridToTriangle.index(ii,jj).push_back(3*(2*(jj)*(m_Nx-1) + (2*ii)));
    m_gridToTriVertex.index(ii,jj).push_back(NE);
}

} // end namespace seng