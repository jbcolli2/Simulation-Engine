//
// Created by jcollin2 on 9/4/22.
//

#include <random>

#include "MeshComponents.h"
#include "Rendering/Mesh.h"

namespace seng
{






std::unordered_map<PrimitiveType, unsigned int> Primitive::m_vao =
        {
                {PrimitiveType::CUBE, 0},
                {PrimitiveType::PLANE, 0},
                {PrimitiveType::SPHERE, 0}
        };
std::unordered_map<PrimitiveType, unsigned int> Primitive::m_vbo =
        {
                {PrimitiveType::CUBE, 0},
                {PrimitiveType::PLANE, 0},
                {PrimitiveType::SPHERE, 0}
        };
std::unordered_map<PrimitiveType, unsigned int> Primitive::m_ebo =
        {
                {PrimitiveType::CUBE, 0},
                {PrimitiveType::PLANE, 0},
                {PrimitiveType::SPHERE, 0}
        };
std::unordered_map<PrimitiveType, unsigned int> Primitive::m_numVerts =
        {
                {PrimitiveType::CUBE, 0},
                {PrimitiveType::PLANE, 0},
                {PrimitiveType::SPHERE, 0}
        };
std::unordered_map<PrimitiveType, unsigned int> Primitive::m_numIndices =
        {
                {PrimitiveType::CUBE, 0},
                {PrimitiveType::PLANE, 0},
                {PrimitiveType::SPHERE, 0}
        };




/***************** StartUp  ******************
 * @brief If parent object does not have Mesh component, create one.
 *
 *      If primitiveType vao has aleady been created, assign it and vbo/ebo to
 *      the Mesh components first MeshData object.
 *
 *      If vao not already created, create temp vertex data and generate the vbo/ebo
 *      and create vao.  Then copy all these ids to the MeshData object.
 *
 *      Take Material from parent object and assign it to the MeshData object.
 *
 * @returns
******************************************************************///
void Primitive::StartUp()
{
    if(!parentObject->HasComponent<Mesh>())
    {
        parentObject->AddComponent(new Mesh());
    }
    Mesh& mesh = parentObject->GetComponent<Mesh>();
    mesh.m_meshes.clear();
    mesh.m_meshes.push_back(std::make_unique<MeshData>());
    MeshData& meshData = *mesh.m_meshes[0];
    meshData.m_material = m_material;

    // If Primitive already created, send data to mesh component
    if(m_vao[m_primitiveType] == 0)
    {
        FillVertexData(m_primitiveType);
    }

    TransferToMeshData(m_primitiveType, meshData);
}


/***************** FillVertexData  ******************
 * @brief Use parameter to determine which free function to call to return the vertex list
 *      for the primitive.
 *
 * @param primitiveType Type of primitive to create.
******************************************************************///
void Primitive::FillVertexData(PrimitiveType primitiveType)
{
    std::vector<Vert3x3x2f> vertices{};
    std::vector<unsigned int> elements{};
    switch (primitiveType) {
        case PrimitiveType::CUBE:
            SetCubeVertexData(vertices, elements);
            break;
        case PrimitiveType::PLANE:
            SetPlaneVertexData(vertices, elements);
            break;
        case PrimitiveType::SPHERE:
            SetSphereVertexData(vertices, elements, m_subdivideIterations);
            break;
        default:
            break;
    }
    GenAndLoadVAO(m_vao[primitiveType], m_vbo[primitiveType], m_ebo[primitiveType], vertices, elements);
    m_numVerts[primitiveType] = vertices.size();
    m_numIndices[primitiveType] = elements.size();
}









/***************** SetCubeVertexData  ******************
 * @brief Free function that returns vector of Vert3x3x2f that represent a
 *      cube [-0.5, 0.5] x [-0.5, 0.5] x [-0.5, 0.5].
 *
 * @returns Vertex list of cube.
******************************************************************///
void SetCubeVertexData(std::vector<Vert3x3x2f>& vertices, std::vector<unsigned int>& elements)
{
    vertices = std::vector<Vert3x3x2f>{
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


    elements = std::vector<unsigned int>(12*3);
    for(int tri = 0; tri < 36; ++tri)
    {
        elements[tri] = tri;
    }

}




/***************** SetPlaneVertexData  ******************
 * @brief Free function that returns vector of Vert3x3x2f that represent a
 *      plane [-0.5, 0.5] x {0} x [-0.5, 0.5].  Plane lives on x-z plane.
 *      Normal points in positive y direction.
 *
 * @returns Vertex list of plane.
******************************************************************///
void SetPlaneVertexData(std::vector<Vert3x3x2f>& vertices, std::vector<unsigned int>& elements)
{
    vertices = std::vector<Vert3x3x2f>{
            Vert3x3x2f(-0.5f, -0.f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f), // bottom left
            Vert3x3x2f(-0.5f, -0.f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),  // top left
            Vert3x3x2f(0.5f, -0.f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),  // bottom right
            Vert3x3x2f(0.5f, -0.f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f), // top right
    };

    elements = std::vector<unsigned int>{
            0, 3, 1,
            0, 2, 3
    };


}










void SetSphereVertexData(std::vector<Vert3x3x2f>& vertices, std::vector<unsigned int>& elements, unsigned int subdivideIters)
{
    const float PI = 3.1415926f;
    const float H_ANGLE = PI / 180 * 72;    // 72 degree = 360 / 5
    const float V_ANGLE = atanf(1.0f / 2);  // elevation = 26.565 degree

    int i1, i2;                             // indices
    float y, xz;                            // coords
    float hAngle1 = -PI / 2 - H_ANGLE / 2;  // start from -126 deg at 1st row
    float hAngle2 = -PI / 2;

    vertices = std::vector<Vert3x3x2f>(12 );    // array of 12 vertices (x,y,z)

    // TODO: Set the UV for the sphere correctly.
    // Top of sphere
    vertices[0] = Vert3x3x2f(0, 1.f, 0.f, 0, 1.f, 0);

    // Verts around the middle
    y  = sinf(V_ANGLE);            // elevation
    xz = cosf(V_ANGLE);            // length on XY plane
    for(int ii = 1; ii <= 5; ++ii)
    {
        vertices[ii] = Vert3x3x2f(xz * cosf(hAngle1), y, xz * sinf(hAngle1), xz * cosf(hAngle1), y, xz * sinf(hAngle1));      // top row
        vertices[ii + 5] = Vert3x3x2f(xz * cosf(hAngle2), -y, xz * sinf(hAngle2), xz * cosf(hAngle2), -y, xz * sinf(hAngle2));      // bottom row

        // next horizontal angles
        hAngle1 += H_ANGLE;
        hAngle2 += H_ANGLE;
    }

    // Bottom of sphere
    vertices[11] = Vert3x3x2f(0, -1.f, 0, 0, -1.f, 0);

    elements = std::vector<unsigned int>{
            // Top
            0, 1, 5,
            0, 5, 4,
            0, 4, 3,
            0, 3, 2,
            0, 2, 1,

            // Upside down tris in middle
            1, 2, 6,
            2, 3, 7,
            3, 4, 8,
            4, 5, 9,
            5, 1, 10,

            // Rightside up tris in middle
            10, 9, 5,
            9, 8, 4,
            8, 7, 3,
            7, 6, 2,
            6, 10, 1,

            // Bottom
            11, 6, 7,
            11, 7, 8,
            11, 8, 9,
            11, 9, 10,
            11, 10, 6
    };

    SubdivideMesh(subdivideIters, vertices, elements);


    /////////////  Set UV coordinates of sphere  ///////////////////
    for(auto& vertex : vertices)
    {
        vertex.s = atan2(-vertex.z, vertex.x)/PI + 0.5f;
        vertex.t = asinf(vertex.y)/PI + 0.5f;
    }

}




void SubdivideMesh(int iterations, std::vector<Vert3x3x2f>& vertices, std::vector<unsigned int>& elements)
{
    std::vector<Vert3x3x2f> baseVerts;
    std::vector<unsigned int> baseElements;
    Vert3x3x2f v1{}, v2{}, v3{};
    Vert3x3x2f newV1{}, newV2{}, newV3{};

    for(int iters = 0; iters < iterations; ++iters)
    {
        baseVerts = vertices;
        baseElements = elements;
        vertices.clear();
        elements.clear();

        int index = 0;

        for(int tri = 0; tri < baseElements.size(); tri += 3)
        {
            v1 = baseVerts[baseElements[tri]];
            v2 = baseVerts[baseElements[tri + 1]];
            v3 = baseVerts[baseElements[tri + 2]];

            computeHalfVertex(v1, v2, newV1);
            scaleToUnit(newV1);
            computeHalfVertex(v2, v3, newV2);
            scaleToUnit(newV2);
            computeHalfVertex(v3, v1, newV3);
            scaleToUnit(newV3);

            vertices.push_back(v1);
            vertices.push_back(newV1);
            vertices.push_back(newV3);

            vertices.push_back(newV1);
            vertices.push_back(v2);
            vertices.push_back(newV2);

            vertices.push_back(newV1);
            vertices.push_back(newV2);
            vertices.push_back(newV3);

            vertices.push_back(newV3);
            vertices.push_back(newV2);
            vertices.push_back(v3);

            for(int ii = 0; ii < 12; ++ii)
            {
                elements.push_back(index + ii);
            }

            index += 12;

        }

    }

}



void computeHalfVertex(const Vert3x3x2f& v1, const Vert3x3x2f& v2, Vert3x3x2f& newV)
{
    newV.x = v1.x + v2.x;
    newV.y = v1.y + v2.y;
    newV.z = v1.z + v2.z;

    newV.r = newV.x;
    newV.g = newV.y;
    newV.b = newV.z;
}



void scaleToUnit(Vert3x3x2f& v)
{
    float scale = 1.f/glm::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);

    v.x *= scale;
    v.y *= scale;
    v.z *= scale;

    v.r = v.x;
    v.g = v.y;
    v.b = v.z;
}








} // seng