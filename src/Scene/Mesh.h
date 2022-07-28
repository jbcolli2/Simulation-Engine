//
// Created by jcollin2 on 7/18/22.
//

#ifndef SIM_ENGINE_MESH_H
#define SIM_ENGINE_MESH_H


#include "Scene/Material.h"
#include "Rendering/Shader.hpp"

namespace seng
{

class Mesh
{
protected:
    unsigned int m_vao{0};
    unsigned int m_vbo{0};
    unsigned int m_ebo{0};
    unsigned int m_numVertices{0};


    Material* m_material{};

public:
    Mesh() = default;

    void virtual SetMaterial(Material* material)
    {
        m_material = material;
    };
    void virtual SetupMaterial(Shader& shader)
    {
        m_material->SetupMaterial(shader);
    };

    void virtual Draw(Shader& shader) = 0;

};


} // end namespace seng


#endif //SIM_ENGINE_MESH_H
