//
// Created by Jeb Collins on 3/4/22.
//

#ifndef PECS_RENDERER_RENDERABLE_HPP
#define PECS_RENDERER_RENDERABLE_HPP

#include "Scene/Mesh.h"
#include "ObjComp/Component.h"

#include <glm/glm.hpp>
#include <vector>

namespace seng
{




/***************** Renderable Component  ******************
 * @brief Contains vector of Mesh objects and model matrix.
******************************************************************///


class Renderable : public Component
{
public:
    std::vector<Mesh*> m_meshes;
    glm::mat4 m_model;

    void Draw()
    {
        for(Mesh* mesh : m_meshes)
        {
            mesh->Draw();
        }
    }

};


} // End seng namespace
#endif //PECS_RENDERER_RENDERABLE_HPP
