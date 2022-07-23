//
// Created by jcollin2 on 7/19/22.
//

#ifndef SIM_ENGINE_CUBE_H
#define SIM_ENGINE_CUBE_H

#include "Misc/Util.h"

#include "Scene/Mesh.h"


namespace seng
{

class Cube : public Mesh
{
private:
    std::vector<Vert3x3x2f> m_vertices;


    void FillVertexData();


    /***************** Cube ctor  ******************
     * @brief Fill the m_vertices vector with cube vertex data.
     *
     *      Generate vao/vbo and set vertex attributes.
     *      Set Material if given.
     *
     * @param material Pointer to material used with mesh.
    ******************************************************************///
    Cube(Material* material = nullptr);
public:
    void SetMaterial(Material* material)
    {
        m_material = material;
    };







    //***********************************************************
    //       Singleton methods
    //***********************************************************

    /***************** GetInstance  ******************
     * @brief Makes this a singleton class.  This allows only one Cube to exist, making sure
     *      all cubes being rendered use the same vao and we don't fill up the VRAM with duplicate
     *      vertex data.
     *
     * @returns Reference to static Cube object.
    ******************************************************************///
    static Cube& GetInstance()
    {
        static Cube instance{};
        return instance;
    };









    //***********************************************************
    //       Virtual methods inherited from Mesh
    //***********************************************************

    /***************** SetupMaterial  ******************
     * @brief Calls the m_material SetupMaterial method to setup the shader and/or textures
     *      for the draw call.
     *
     * @param shader Shader used for the draw.
    ******************************************************************///
    void virtual SetupMaterial(Shader& shader);



    /***************** Draw  ******************
     * @brief Binds VAO and calls glDraw... to draw the mesh.
    ******************************************************************///
    void virtual Draw(Shader& shader);
};

} // end namespace seng

#endif //SIM_ENGINE_CUBE_H
