//
// Created by jcollin2 on 7/19/22.
//

#include "Primitive.h"

namespace seng
{

/***************** Primitive ctor  ******************
 * @brief Fill the m_vertices vector with cube vertex data.
 *
 *      Generate vao/vbo and set vertex attributes.
 *      Set Material if given.
 *
 * @param material Pointer to material used with mesh.
******************************************************************///
Primitive::Primitive(PrimitiveType primitiveType)
{
    m_material = nullptr;


    ///////////////// Setup vertex data for cube ///////////////////////////////////////
    FillVertexData(primitiveType);

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
}






/***************** Draw  ******************
 * @brief Binds VAO and calls glDraw... to draw the mesh.
******************************************************************///
void Primitive::Draw(Shader& shader)
{
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
}


/***************** FillVertexData  ******************
 * @brief Use parameter to determine which free function to call to return the vertex list
 *      for the primitive.
 *
 * @param primitiveType Type of primitive to create.
 *
 *
 * Fills m_vertices with data for a standard cube:
 *      [-.5, .5] x [-.5, .5] x [-.5, .5]
 *      with normal and UV data.
******************************************************************///
void Primitive::FillVertexData(PrimitiveType primitiveType)
{
    switch (primitiveType) {
        case PrimitiveType::CUBE:
            m_vertices = GetCubeVertexList();
            break;
        case PrimitiveType::PLANE:
            m_vertices = GetPlaneVertexList();
            break;
        case PrimitiveType::SPHERE:
            m_vertices = GetSphereVertexList();
            break;
        default:
            break;
    }
}




/***************** GetCubeVertexList  ******************
 * @brief Free function that returns vector of Vert3x3x2f that represent a
 *      cube [-0.5, 0.5] x [-0.5, 0.5] x [-0.5, 0.5].
 *
 * @returns Vertex list of cube.
******************************************************************///
std::vector<Vert3x3x2f> GetCubeVertexList()
{
    std::vector<Vert3x3x2f> verts = {
            Vert3x3x2f(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f), // front
            Vert3x3x2f(0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f),
            Vert3x3x2f(-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f),

            Vert3x3x2f(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
            Vert3x3x2f(0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f),
            Vert3x3x2f(0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f),

            Vert3x3x2f(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f), // back
            Vert3x3x2f(-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
            Vert3x3x2f(0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),

            Vert3x3x2f(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
            Vert3x3x2f(0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),
            Vert3x3x2f(0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),

            Vert3x3x2f(-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f), //top
            Vert3x3x2f(0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),
            Vert3x3x2f(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),

            Vert3x3x2f(0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),
            Vert3x3x2f(0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f),
            Vert3x3x2f(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),

            Vert3x3x2f(-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f), //bottom
            Vert3x3x2f(-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f),
            Vert3x3x2f(0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f),

            Vert3x3x2f(0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f),
            Vert3x3x2f(-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f),
            Vert3x3x2f(0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f),

            Vert3x3x2f(0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f), //right
            Vert3x3x2f(0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
            Vert3x3x2f(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),

            Vert3x3x2f(0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
            Vert3x3x2f(0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
            Vert3x3x2f(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),

            Vert3x3x2f(-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f), //left
            Vert3x3x2f(-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
            Vert3x3x2f(-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f),

            Vert3x3x2f(-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
            Vert3x3x2f(-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
            Vert3x3x2f(-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
    };

    return verts;
}





/***************** GetPlaneVertexList  ******************
 * @brief Free function that returns vector of Vert3x3x2f that represent a
 *      plane [-0.5, 0.5] x {0} x [-0.5, 0.5].  Plane lives on x-z plane.
 *      Normal points in positive y direction.
 *
 * @returns Vertex list of plane.
******************************************************************///
std::vector<Vert3x3x2f> GetPlaneVertexList()
{
    std::vector<Vert3x3x2f> verts = {
            Vert3x3x2f(-0.5f, -0.f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
            Vert3x3x2f(-0.5f, -0.f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
            Vert3x3x2f(0.5f, -0.f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),

            Vert3x3x2f(-0.5f, -0.f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
            Vert3x3x2f(0.5f, -0.f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
            Vert3x3x2f(0.5f, -0.f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
    };

    return verts;
}



std::vector<Vert3x3x2f> GetSphereVertexList()
{
    return std::vector<Vert3x3x2f>{};
}
















} // seng