//
// Created by jcollin2 on 9/4/22.
//

#ifndef SIM_ENGINE_MESHCOMPONENTS_H
#define SIM_ENGINE_MESHCOMPONENTS_H

#include "Engine/Component.h"
#include "Rendering/Mesh.h"

namespace seng
{

//***********************************************************
//       Vertex filling functions for Primitives
//***********************************************************
void SetCubeVertexData(std::vector<Vert3x3x2f>& vertices, std::vector<unsigned int>& elements);
void SetPlaneVertexData(std::vector<Vert3x3x2f>& vertices, std::vector<unsigned int>& elements);
void SetSphereVertexData(std::vector<Vert3x3x2f>& vertices, std::vector<unsigned int>& elements);


void Subdivide(int iterations, std::vector<Vert3x3x2f>& vertices, std::vector<unsigned int>& elements);
void computeHalfVertex(Vert3x3x2f& v1, Vert3x3x2f& v2, Vert3x3x2f& newV);
void scaleToUnit(Vert3x3x2f& v);



enum class PrimitiveType
{
    CUBE,
    PLANE,
    SPHERE
};

class Material;
class Primitive : public Component
{
private:
    /////////////  Members of Primitive  ///////////////////
    PrimitiveType m_primitiveType;      // Type of primitive (CUBE, PLANE, ...)
    Material* m_material;               // Material to use on the primitive.  Only one material to be used.

    // Static VAO/VBO for each primitive
    // Use same VAO for each primitive created in game
    static std::unordered_map<PrimitiveType, unsigned int> m_vao, m_vbo, m_ebo, m_numVerts, m_numIndices;
//    static unsigned int m_cubeVAO, m_cubeVBO, m_cubeEBO, m_cubeNumVert, m_cubeNumIndices;
//    static unsigned int m_planeVAO, m_planeVBO, m_planeEBO, m_planeNumVert, m_planeNumIndices;
//    static unsigned int m_sphereVAO, m_sphereVBO, m_sphereEBO, m_sphereNumVert, m_sphereNumIndices;


    /***************** FillVertexData  ******************
     * @brief Generate vertex and element data for the passes primitive type.  Generate the
     *      vao, vbo, and ebo for the primitive and fill with vertex and element data.
     *
     *      Set attributes for vao according to Vert3x3x2f.
     *
     *      Unbind vao.
     *
     * @param primitiveType CUBE, PLANE, SPHERE, ...
    ******************************************************************///
    void FillVertexData(PrimitiveType primitiveType);


    /***************** TransferToMeshdata  ******************
     * @brief For passed primitive, transfer all the vao, vbo, ... data to passed MeshData object
     *
     * @param primitiveType Type of primitive
     * @param meshdata MeshData object to be filling with primitive's vertex rendering data
    ******************************************************************///
    void TransferToMeshData(PrimitiveType primitiveType, MeshData& meshData)
    {
        meshData.m_vbo = m_vbo[m_primitiveType];
        meshData.m_vao = m_vao[m_primitiveType];
        meshData.m_ebo = m_ebo[m_primitiveType];
        meshData.m_numVertices = m_numVerts[m_primitiveType];
        meshData.m_numIndices = m_numIndices[m_primitiveType];
    }


public:
    Primitive(PrimitiveType primitiveType, Material* material) : m_primitiveType(primitiveType), m_material(material) {};
    void StartUp() override;
};






//***********************************************************
//       Node class for masses in cloth
//***********************************************************
struct Node
{
    glm::vec3 currentPosition;
    glm::vec3 prevPosition;
    std::vector<int> adjIndices;
    bool fixedFlag{false};
};

class Mesh;
//***********************************************************
//       System for rod-based cloth simulation
//***********************************************************
class RodCloth : public Component
{
private:

    int m_Nx{0}, m_Ny{0};                               // Dimensions of the GridMesh
    float m_mass{1.f};                                  // Mass of all nodes making up the cloth
    int m_numConstraintIterations{5};                   // Number of iterations to use when projecting the constraints

    std::vector<Node> m_nodes{};                          // State data for each node in the cloth
    std::vector<int> m_shuffleIndex{};                  // Shuffled indices to access the nodes in random order


    float m_rodLength{1.f};                             // Cache the length of all axis-aligned rods connecting masses
                                                        // TODO: Change to allow for different lenghts in each direction
    // TODO: Add diagonal rod length


    const glm::vec3 m_g{glm::vec3(0.f, -9.8f, 0.f)};    // Value of acceleration due to gravity




    // pointer to the grid mesh
    Mesh* m_gridMesh;

    int mat2VecIdx(int ii, int jj)
    {
        return Matrix2VectorIdx(ii, jj, m_Nx);
    };

    enum class InitialClothState
    {
        VERTICAL_FIX_ENDS
    };

    //***********************************************************
    //       Methods to break up StartUp and Update
    //***********************************************************
    void InitializeNodes(InitialClothState initState);
    void Integrate(float deltaTime);
    void ProjectConstraints();
    void SetVertexData();


public:
    RodCloth(int numXNodes, int numYNodes, float mass = 1.f, unsigned int numIterations = 5);

    void StartUp() override;

    void Update(float deltaTime) override;

    void ShutDown() override;
}; // End RodCloth class


} // seng

#endif //SIM_ENGINE_MESHCOMPONENTS_H
