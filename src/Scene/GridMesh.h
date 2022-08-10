//
// Created by jcollin2 on 7/29/22.
//

#ifndef SIM_ENGINE_GRIDMESH_H
#define SIM_ENGINE_GRIDMESH_H

#include "Misc/Common.h"

#include "Scene/Mesh.h"

namespace seng
{








class Shader;

class GridMesh : public Mesh
{
private:
    static constexpr int SW{0};
    static constexpr int S{2};
    static constexpr int SE{1};
    static constexpr int NW{1};
    static constexpr int N{2};
    static constexpr int NE{0};
    void addSWTriangle(int ii, int jj);
    void addSTriangle(int ii, int jj);
    void addSETriangle(int ii, int jj);
    void addNWTriangle(int ii, int jj);
    void addNTriangle(int ii, int jj);
    void addNETriangle(int ii, int jj);

    int m_Nx{0}, m_Ny{0};
    std::vector<Vert3x3x2f> m_vertices;  //Rendering vertex list
    Grid<std::vector<int>> m_gridToTriangle;  // (i,j) to first index of triangles
    Grid<std::vector<int>> m_gridToTriVertex; // (i,j) to vertex within triangle

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
    void setupGridToVertices();



    /***************** computeNormals  ******************
     * @brief Computes the normals based on the position of each
     *      of the points on the grid.  Normals are computed as the average of
     *      the normals of all triangles that use that point.
    ******************************************************************///
    void computeNormals();

public:
    GridMesh(int N_x = 10, int N_y = 10);

    // Position of points on the grid
    Grid<glm::vec3> m_gridPos;




    /***************** SetupForDraw  ******************
     * @brief Setup the vertex data after changeing the grid positions.  Also calculate the normals.
    ******************************************************************///
    void SetupForDraw();




    void virtual Draw(Shader& shader) override;




    //***********************************************************
    //       Getter/Setter
    //***********************************************************
    int GetNx() {return m_Nx;};
    int GetNy() {return m_Ny;};
};

} // end namespace seng


#endif //SIM_ENGINE_GRIDMESH_H
