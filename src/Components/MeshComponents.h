//
// Created by jcollin2 on 9/4/22.
//

#ifndef SIM_ENGINE_MESHCOMPONENTS_H
#define SIM_ENGINE_MESHCOMPONENTS_H

#include "Misc/MeshCommon.h"
#include "Engine/Component.h"
#include "Rendering/Mesh.h"

namespace seng
{

//***********************************************************
//       Vertex filling functions for Primitives
//***********************************************************
void SetCubeVertexData(std::vector<Vert3x3x2f>& vertices, std::vector<unsigned int>& elements);
void SetPlaneVertexData(std::vector<Vert3x3x2f>& vertices, std::vector<unsigned int>& elements);
void SetSphereVertexData(std::vector<Vert3x3x2f>& vertices, std::vector<unsigned int>& elements, unsigned int subdivideIters);


void SubdivideMesh(int iterations, std::vector<Vert3x3x2f>& vertices, std::vector<unsigned int>& elements);
void computeHalfVertex(const Vert3x3x2f& v1, const Vert3x3x2f& v2, Vert3x3x2f& newV);
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

    // Static mesh data for each primitive
    // Use same VAO for each primitive created in game
    static std::unordered_map<PrimitiveType, std::string> m_primitiveVAOKey;
    static std::unordered_map<PrimitiveType, std::vector<Vert3x3x2f>> m_primitiveVertList;
    static std::unordered_map<PrimitiveType, std::vector<unsigned int>> m_primitiveElementList;
    static unsigned int m_subdivideIterations; // Number of times to subdivide for those that use it (i.e. sphere)


    /***************** GeneratePrimitiveMesh  ******************
     * @brief Generate vertex and element data for the passes primitive type.  Generate the
     *      vao, vbo, and ebo for the primitive and fill with vertex and element data.
     *
     *      Set attributes for vao according to Vert3x3x2f.
     *
     *      Unbind vao.
     *
     * @param primitiveType CUBE, PLANE, SPHERE, ...
    ******************************************************************///
    void GeneratePrimitiveMesh(PrimitiveType primitiveType);




public:
    Primitive(PrimitiveType primitiveType, Material* material) :
    m_primitiveType(primitiveType), m_material(material) {};
    void StartUp() override;


    static void SetSubdivideIterations(unsigned int subdivideIterations);
};








} // seng

#endif //SIM_ENGINE_MESHCOMPONENTS_H
