//
// Created by jcollin2 on 7/19/22.
//

#ifndef SIM_ENGINE_PRIMITIVEMESH_H
#define SIM_ENGINE_PRIMITIVEMESH_H

#include "Misc/Common.h"

#include "Scene/Mesh.h"


namespace seng
{

enum class PrimitiveType
{
    CUBE,
    PLANE,
    SPHERE
};




//***********************************************************
//       Vertex filling functions for Primitives
//***********************************************************
void SetCubeVertexData(std::vector<Vert3x3x2f>& vertices, std::vector<unsigned int>& elements);
void SetPlaneVertexData(std::vector<Vert3x3x2f>& vertices, std::vector<unsigned int>& elements);
void SetSphereVertexData(std::vector<Vert3x3x2f>& vertices, std::vector<unsigned int>& elements);


void Subdivide(int iterations, std::vector<Vert3x3x2f>& vertices, std::vector<unsigned int>& elements);
void computeHalfVertex(Vert3x3x2f& v1, Vert3x3x2f& v2, Vert3x3x2f& newV);
void scaleToUnit(Vert3x3x2f& v);





class PrimitiveMesh : public Mesh
{
private:
    std::vector<Vert3x3x2f> m_vertices{}; // List of all vertices in the primitive
    std::vector<unsigned int> m_elements{};

    // Static VAO/VBO for each primitive
    // Use same VAO for each primitive created in game
    static unsigned int m_cubeVAO, m_cubeVBO, m_cubeEBO;
    static unsigned int m_planeVAO, m_planeVBO, m_planeEBO;
    static unsigned int m_sphereVAO, m_sphereVBO, m_sphereEBO;


    void FillVertexData(PrimitiveType primitiveType);
    void FillElementData(PrimitiveType primitiveType);



public:
    /***************** PrimitiveMesh ctor  ******************
     * @brief Fill the m_vertices vector with cube vertex data.
     *
     *      Generate vao/vbo and set vertex attributes.
     *      Set Material if given.
     *
     * @param material Pointer to material used with mesh.
    ******************************************************************///
    PrimitiveMesh(PrimitiveType primitiveType);




    //***********************************************************
    //       Virtual methods from Mesh
    //***********************************************************

    /***************** Draw  ******************
     * @brief Binds VAO and calls glDraw... to draw the mesh.
    ******************************************************************///
    void virtual Draw(Shader& shader);
};


} // end namespace seng

#endif //SIM_ENGINE_PRIMITIVEMESH_H
