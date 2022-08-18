//
// Created by jcollin2 on 7/19/22.
//

#include "PrimitiveMesh.h"

namespace seng
{

unsigned int PrimitiveMesh::m_cubeVAO{0};
unsigned int PrimitiveMesh::m_cubeVBO{0};
unsigned int PrimitiveMesh::m_cubeEBO{0};
unsigned int PrimitiveMesh::m_planeVAO{0};
unsigned int PrimitiveMesh::m_planeVBO{0};
unsigned int PrimitiveMesh::m_planeEBO{0};
unsigned int PrimitiveMesh::m_sphereVAO{0};
unsigned int PrimitiveMesh::m_sphereVBO{0};
unsigned int PrimitiveMesh::m_sphereEBO{0};




/***************** PrimitiveMesh ctor  ******************
 * @brief Fill the m_vertices vector with cube vertex data.
 *
 *      Generate vao/vbo and set vertex attributes.
 *      Set Material if given.
 *
 * @param material Pointer to material used with mesh.
******************************************************************///
PrimitiveMesh::PrimitiveMesh(PrimitiveType primitiveType)
{
    m_material = nullptr;



    // Check to see if the vao/vbo for this primitive has already been created
    //   if so, then just set vao/vbo to static vao/vbo and done!
    if(primitiveType == PrimitiveType::CUBE && m_cubeVAO != 0)
    {
        m_vao = m_cubeVAO;
        m_vbo = m_cubeVBO;
        m_ebo = m_cubeEBO;
        return;
    }
    if(primitiveType == PrimitiveType::PLANE && m_planeVAO != 0)
    {
        m_vao = m_planeVAO;
        m_vbo = m_planeVBO;
        m_ebo = m_planeEBO;
        return;
    }
    if(primitiveType == PrimitiveType::SPHERE && m_sphereVAO != 0)
    {
        m_vao = m_sphereVAO;
        m_vbo = m_sphereVBO;
        m_ebo = m_sphereEBO;
        return;
    }


    ///////////////// Setup vertex data for primitive ///////////////////////////////////////
    FillVertexData(primitiveType);
    FillElementData(primitiveType);

    m_numVertices = m_vertices.size();

    //******* VBO/VAO   ***************
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    loadDataToVBO(m_vbo, m_vertices);
    loadDataToEBO(m_ebo, m_elements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    SetVertexAttribs<Vert3x3x2f>();

    glBindVertexArray(0);
}






/***************** Draw  ******************
 * @brief Binds VAO and calls glDraw... to draw the mesh.
******************************************************************///
void PrimitiveMesh::Draw(Shader& shader)
{
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_elements.size(), GL_UNSIGNED_INT, 0);
}


/***************** FillVertexData  ******************
 * @brief Use parameter to determine which free function to call to return the vertex list
 *      for the primitive.
 *
 * @param primitiveType Type of primitive to create.
 *
 *
******************************************************************///
void PrimitiveMesh::FillVertexData(PrimitiveType primitiveType)
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




void PrimitiveMesh::FillElementData(PrimitiveType primitiveType)
{
    switch (primitiveType) {
        case PrimitiveType::CUBE:
            m_elements = GetCubeElementList();
            break;
        case PrimitiveType::PLANE:
            m_elements = GetPlaneElementList();
            break;
        case PrimitiveType::SPHERE:
            m_elements = GetSphereElementList();
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

std::vector<unsigned int> GetCubeElementList()
{
    std::vector<unsigned int> elements(12*3);

    for(int tri = 0; tri < 36; ++tri)
    {
        elements[tri] = tri;
    }

    return elements;
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
            Vert3x3x2f(-0.5f, -0.f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f), // bottom left
            Vert3x3x2f(-0.5f, -0.f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),  // top left
            Vert3x3x2f(0.5f, -0.f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),  // bottom right
            Vert3x3x2f(0.5f, -0.f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f), // top right
    };

    return verts;
}


std::vector<unsigned int> GetPlaneElementList()
{
    std::vector<unsigned int> elements = {
            0, 3, 1,
            0, 2, 3
    };

    return elements;
}








std::vector<Vert3x3x2f> GetSphereVertexList()
{
    const float PI = 3.1415926f;
    const float H_ANGLE = PI / 180 * 72;    // 72 degree = 360 / 5
    const float V_ANGLE = atanf(1.0f / 2);  // elevation = 26.565 degree

    int i1, i2;                             // indices
    float z, xy;                            // coords
    float hAngle1 = -PI / 2 - H_ANGLE / 2;  // start from -126 deg at 1st row
    float hAngle2 = -PI / 2;

    std::vector<Vert3x3x2f> vertices(10 );    // array of 12 vertices (x,y,z)

    // Top tris


    return vertices;

}


std::vector<unsigned int> GetSphereElementList()
{
    return std::vector<unsigned int>(3);
}
















} // seng