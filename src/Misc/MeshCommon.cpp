//
// Created by jcollin2 on 9/29/22.
//

#include "MeshCommon.h"

#include "Rendering/Mesh.h"
#include "DynamicMeshCommon.h"

namespace seng
{


template<>
void SetVertexAttribs<Vert3x3x2f>()
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

};






VAO::VAO(VAO&& otherVAO) : m_vaoID(otherVAO.m_vaoID), m_vboID(otherVAO.m_vboID), m_eboID(otherVAO.m_eboID),
                           m_numVerts(otherVAO.m_numVerts), m_numElements(otherVAO.m_numElements)
{
    resetVAO();
}

VAO& VAO::operator=(VAO&& otherVAO)
{
    // Check for self assignment
    if(&otherVAO == this)
        return *this;

    otherVAO.m_vaoID = m_vaoID;
    otherVAO.m_vboID = m_vboID;
    otherVAO.m_eboID = m_eboID;
    otherVAO.m_numVerts = m_numVerts;
    otherVAO.m_numElements = m_numElements;

    resetVAO();
}

VAO::~VAO()
{
    glDeleteVertexArrays(1, &m_vaoID);
    glDeleteBuffers(1, &m_vboID);
    glDeleteBuffers(1, &m_eboID);
}


} // seng