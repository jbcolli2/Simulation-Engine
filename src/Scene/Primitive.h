//
// Created by jcollin2 on 7/19/22.
//

#ifndef SIM_ENGINE_PRIMITIVE_H
#define SIM_ENGINE_PRIMITIVE_H

#include "Misc/Util.h"

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
std::vector<Vert3x3x2f> GetCubeVertexList();
std::vector<Vert3x3x2f> GetPlaneVertexList();
std::vector<Vert3x3x2f> GetSphereVertexList();








class Primitive : public Mesh
{
private:
    std::vector<Vert3x3x2f> m_vertices;


    void FillVertexData(PrimitiveType primitiveType);



    /***************** Primitive ctor  ******************
     * @brief Fill the m_vertices vector with cube vertex data.
     *
     *      Generate vao/vbo and set vertex attributes.
     *      Set Material if given.
     *
     * @param material Pointer to material used with mesh.
    ******************************************************************///
    Primitive(PrimitiveType primitiveType);
public:

    /***************** GetInstance  ******************
     * @brief Makes this a singleton class.  This allows only one Primitive to exist, making sure
     *      all cubes being rendered use the same vao and we don't fill up the VRAM with duplicate
     *      vertex data.
     *
     * @returns Reference to static Primitive object.
    ******************************************************************///
    static Primitive& GetInstance(PrimitiveType primitiveType)
    {
        static Primitive instance{primitiveType};
        return instance;
    };




    //***********************************************************
    //       Virtual methods from Mesh
    //***********************************************************

    /***************** Draw  ******************
     * @brief Binds VAO and calls glDraw... to draw the mesh.
    ******************************************************************///
    void virtual Draw(Shader& shader);
};


} // end namespace seng

#endif //SIM_ENGINE_PRIMITIVE_H
