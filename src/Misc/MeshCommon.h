//
// Created by jcollin2 on 9/29/22.
//

#ifndef SIM_ENGINE_MESHCOMMON_H
#define SIM_ENGINE_MESHCOMMON_H
#include "Misc/Common.h"
#include "assimp/vector3.h"


namespace seng
{




//***********************************************************
//       MeshData Data
//***********************************************************
struct Vert3x3x2f
{
    float x,y,z;
    float r,g,b;
    float s,t;

    explicit Vert3x3x2f(float x = 0.f, float y = 0.f, float z = 0.f, float r = 0.f, float g = 0.f, float b = 0.f,
                        float s = 0.f, float t = 0.f) :
            x(x), y(y), z(z), r(r), g(g), b(b), s(s), t(t) {};
};


/***************** SetVertexAttribs  ******************
 * @brief Set the vertex attributes for a particular type of vertex data.  The desired
 *      VAO must be bound before calling this.
 *
 * @tparam vertexType Pass in the vertex type to determine what attribute to set for the vertex buffer.
******************************************************************///
template <typename vertexType>
void SetVertexAttribs();




/***************** VAO  ******************
 * @brief Object that represents the vertex array object in OpenGL.  Holds the handle needed for draw calls as
 *      well as the number of vertices and elements.  Also holds the handle for the vertex buffer object and element buffer object.
 *
 *      Implements using RAII, so the buffers are created at initialization of this object.
 *      Also does not allow copy semantics as that would create a copy of the buffers in OpenGL.  Implements move
 *      semantics.
 *
 *      Dtor destroys the buffers.
******************************************************************///
class VAO
{
private:
    void resetVAO()
    {
        m_vaoID = 0;
        m_vboID = 0;
        m_eboID = 0;
        m_numVerts = 0;
        m_numElements = 0;
    }


public:
    unsigned int m_vaoID{0}, m_vboID{0}, m_eboID{0}, m_numVerts{0}, m_numElements{0};

    // Create an empty VAO
    VAO() = default;

    template<typename VertT, typename ElementT>
    VAO(std::vector<VertT> vertexList = std::vector<VertT>(), std::vector<ElementT> elementList = std::vector<ElementT>(), GLenum drawStyle= GL_STATIC_DRAW);

    // Delete the Copy Semantics
    VAO(const VAO& otherVAO) = delete;
    VAO& operator=(const VAO& otherVAO) = delete;

    // Move Semantics
    VAO(VAO&& otherVAO);
    VAO& operator=(VAO&& otherVAO);

    // Dtor
    ~VAO();



    bool IsEmpty()
    {
        return m_numVerts == 0;
    }


    /***************** PrintData  ******************
     * @brief A pretty-print of all the vao data.  For debugging purposes.
     *
     *      TODO: Add a stretch parameter to control how wide the print is.
    ******************************************************************///
    void PrintData()
    {
        std::string space = "       ";
        std::cout << "=====  VAO  |  VBO  |  EBO  |  #Vert  |  #Elements  =====\n";
        std::cout << "        " << m_vaoID << space << m_vboID << space << m_eboID << space << m_numVerts << space << "    " <<  m_numElements << "\n";
    }

};


template<typename VertT, typename ElementT>
VAO::VAO(std::vector<VertT> vertexList, std::vector<ElementT> elementList, GLenum drawStyle) :
    m_numVerts(vertexList.size()), m_numElements(elementList.size())
{
    glGenVertexArrays(1, &m_vaoID);
    glBindVertexArray(m_vaoID);
    glGenBuffers(1, &m_vboID);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    glBufferData(GL_ARRAY_BUFFER, vertexList.size()*sizeof(VertT), &vertexList[0], drawStyle);
    if(elementList.size() > 0)
    {
        glGenBuffers(1, &m_eboID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementList.size()*sizeof(ElementT), &elementList[0], drawStyle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboID);
    }
    SetVertexAttribs<VertT>();

    glBindVertexArray(0);

}


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




class MeshData;

} // seng

#endif //SIM_ENGINE_MESHCOMMON_H
