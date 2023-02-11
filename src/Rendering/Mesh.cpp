//
// Created by jcollin2 on 9/19/22.
//


#include "Rendering/Mesh.h"
#include "Rendering/Material.h"


namespace seng
{

void MeshData::Render(Shader& shader)
{
    m_material->SetupMaterial(shader);
    glBindVertexArray(m_vao.m_vaoID);
    if(m_vao.m_eboID != 0)
    {
        glDrawElements(GL_TRIANGLES, m_vao.m_numElements, GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, m_vao.m_numVerts);
    }
    m_material->ResetMaterial(shader);
}







}