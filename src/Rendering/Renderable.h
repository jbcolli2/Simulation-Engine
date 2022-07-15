//
// Created by Jeb Collins on 3/4/22.
//

#ifndef PECS_RENDERER_RENDERABLE_HPP
#define PECS_RENDERER_RENDERABLE_HPP

#include "ObjComp/Component.h"
//#include "RenderUtil.hpp"

#include <glm/glm.hpp>
#include <vector>

namespace seng
{




/***************** Renderable Component  ******************
 * @brief An object with a @Renderable component is any object that
 *      has vertex data associated with it.  This does not mean it has any textures or even materials.
 *
 *      When the pipeline processes a @Renderable component, it knows it has vertices to send
 *      on to the shader.
******************************************************************///


class Renderable : public Component
{
public:
//    // TODO: Possibly move the vertices and vao, vbo to private and set with accessor method.
//    // List of all vertices for this renderable
//    // TODO: This is using 3x3x2f as the hardcoded attributes for the vertex.  What if we wanted different attributes?
//    std::vector<Vert3x3x2f> vertices{};
//    // Vertex Array Object/ Vertex Buffer Object
//    unsigned int vao{0}, vbo{0};
//    // Bool stating if the normals stored in @vertices should be flipped before rendering
//    bool flipNormals;
//    // The type of the Renderable (i.e. Cube, Plane, Model, ...)
//    RenderType renderType;
//    // path the the .obj model file (Optional, only used for Model type)
//    std::string modelPath{""};
//
//
//    Renderable() = default;
//    virtual ~Renderable() = default;
//    /***************** Renderable Ctor  ******************
//     * @brief Constructor that just sets the type and the flipNormal flag.
//    ***************************************************///
//    Renderable(RenderType type, bool flipNormals = false, const std::string& modelPath = "") :
//    renderType(type), flipNormals(flipNormals), modelPath(modelPath) {};

};


} // End seng namespace
#endif //PECS_RENDERER_RENDERABLE_HPP
