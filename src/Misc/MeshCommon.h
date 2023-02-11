//
// Created by jcollin2 on 9/29/22.
//

#ifndef SIM_ENGINE_MESHCOMMON_H
#define SIM_ENGINE_MESHCOMMON_H
#include "Misc/Common.h"


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

    template<typename VertT, typename ElementT>
    VAO(std::vector<VertT> vertexList, std::vector<ElementT> elementList = std::vector<ElementT>(), GLenum drawStyle= GL_STATIC_DRAW);

    VAO(const VAO& otherVAO) = delete;
    VAO& operator=(const VAO& otherVAO) = delete;
    VAO(VAO&& otherVAO);
    VAO& operator=(VAO&& otherVAO);
    ~VAO();
};


template<typename VertT, typename ElementT>
VAO::VAO(std::vector<VertT> vertexList, std::vector<ElementT> elementList, GLenum drawStyle)
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
    SetVertexAttribs<Vert3x3x2f>();

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
class DynamicGridMesh
{
private:
    int m_Nx{0}, m_Ny{0};                               // Number of vertices in x and y axes
    std::vector<Vert3x3x2f> m_vertices;                 //Rendering vertex list

    std::unordered_map<int, glm::vec3> m_cachedNormals{}; // Cache the normal of each triangle as it is computed




    /***************** computeNormals  ******************
     * @brief Computes the normals based on the position of each
     *      of the points on the grid.  Normals are computed as the average of
     *      the normals of all triangles that use that point.
    ******************************************************************///
    void computeNormals();

    /***************** computeTriangleNormal  ******************
     * @brief The grunt work for incrementNormal**.  Compute the normal of a given
     *      triangle defined by vertices vert, edge0Vert, and edge1Vert, which are
     *      passed in by the incrementNormal** methods.
     *
     * @param vertIdx Index into m_vertices of center vertex
     * @param edge0VertIdx Index of vertex to define edge0 of triangle
     * @param edge1VertIdx Index of vertex to define edge1 of triangle
     *
     * @returns Normal of the triangle
    ******************************************************************///
    glm::vec3 computeTriangleNormal(int vertIdx, int edge0VertIdx, int edge1VertIdx);

    /***************** incrementNormalS/SE/SW/...  ******************
     * @brief Compute the normal for one particular triangle, either S, SW, SE, N, NW, or NE.
     *
     *      Add that normal to incVector.
     *
     * @param vert Index of center vertex that we are averaging around
     * @param incVector Vector to increment by computed normal
     *
    ******************************************************************///
    void incrementNormalS(int vertIdx, glm::vec3& incVector)
    {
        incVector += computeTriangleNormal(vertIdx, vertIdx - m_Nx, vertIdx - m_Nx+1);
    }
    void incrementNormalSE(int vertIdx, glm::vec3& incVector)
    {
        incVector += computeTriangleNormal(vertIdx, vertIdx - m_Nx+1, vertIdx +1);
    }
    void incrementNormalSW(int vertIdx, glm::vec3& incVector)
    {
        incVector += computeTriangleNormal(vertIdx, vertIdx - 1, vertIdx - m_Nx);
    }
    void incrementNormalNE(int vertIdx, glm::vec3& incVector)
    {
        incVector += computeTriangleNormal(vertIdx, vertIdx+1, vertIdx +m_Nx);
    }
    void incrementNormalN(int vertIdx, glm::vec3& incVector)
    {
        incVector += computeTriangleNormal(vertIdx, vertIdx +m_Nx, vertIdx +m_Nx-1);
    }
    void incrementNormalNW(int vertIdx, glm::vec3& incVector)
    {
        incVector += computeTriangleNormal(vertIdx, vertIdx + m_Nx-1, vertIdx -1);
    }

    /***************** mat2VecIdx  ******************
     * @brief Convert (ii,jj) matrix indices into vector index assuming matrix has m_Nx columns.
     *
     * @returns Vector index associated with (ii,jj) matrix indices.
    ******************************************************************///
    int mat2VecIdx(int ii, int jj)
    {
        return Matrix2VectorIdx(ii, jj, m_Nx);
    };



public:
    DynamicGridMesh(int Nx, int Ny) : m_Nx(Nx), m_Ny(Ny), m_vertices{std::vector<Vert3x3x2f>(Nx*Ny)} {};

    /***************** GenerateBuffers  ******************
     * @brief Generate the vao/vbo/ebo for this mesh.  Fill the vbo with trash, but fill the ebo
     *      with the correct indices for a square mesh.  Return all handles though output parameters.
    ******************************************************************///
    void GenerateBuffers(MeshData& meshdata);

    /***************** ReloadVBO  ******************
     * @brief As position of the vertices change, update m_vertices and then load the data
     *      into the passed vbo.
     *
     * @param vbo vertex buffer to update with new vertex data
     * @param nodes Data of the vertex nodes from a physical component like cloth.  Data will have currentPosition
     *              member to update position of the vertices.
    ******************************************************************///
    void ReloadVBO(unsigned int vbo, const std::vector<Node>& nodes);


    int GetNumVertices() {return m_vertices.size();};
    int GetNumElements() {return 6*(m_Nx-1)*(m_Ny-1);};
};

} // seng

#endif //SIM_ENGINE_MESHCOMMON_H
