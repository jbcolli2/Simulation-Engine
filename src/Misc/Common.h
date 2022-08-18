//
// Created by jcollin2 on 7/3/22.
//

#ifndef SIM_ENGINE_COMMON_H
#define SIM_ENGINE_COMMON_H


#include <cstdint>
#include <bitset>
#include <typeinfo>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>




namespace seng
{


//***********************************************************
//       Component Information
//***********************************************************
constexpr int MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;



//***********************************************************
//       I/O Methods
//***********************************************************

/***************** loadFile2String  ******************
 * @brief Opens the file at @path and loads returns the contents as a string.
 *
 * @param path The full path of the file to be opened.
 *
 * @returns The contents of the file at path.
******************************************************************/
std::string loadFile2String(const char *path);





/***************** Grid class  ******************
 * @brief Basic 2D dynamic array.  Access elements through index method.
 *      Array is fixed sized, and elements are dynamically allocated on the heap.
******************************************************************///
template <class T>
class Grid
{
    T* m_data{nullptr};
    int N_x,N_y;
public:
    Grid(int M=0, int N=0) : N_x(M), N_y(N) { m_data = new T[M * N];};
    ~Grid() {delete[] m_data;};
    /////////////////    Copy    ///////////////////////
    Grid(const Grid& grid)
    {
        N_x = grid.N_x;
        N_y = grid.N_y;
        m_data = new T[N_x * N_y];
        for(int ii = 0; ii < N_x; ++ii)
        {
            for(int jj = 0; jj < N_y; ++jj)
            {
                m_data[jj * N_x + ii] = grid.index(ii, jj);
            }
        }
    };
    Grid& operator=(const Grid& grid)
    {
        if(&grid == this)
            return *this;

        delete[] m_data;
        N_x = grid.N_x;
        N_y = grid.N_y;
        m_data = new T[N_x * N_y];
        for(int ii = 0; ii < N_x; ++ii)
        {
            for(int jj = 0; jj < N_y; ++jj)
            {
                m_data[jj * N_x + ii] = grid.index(ii, jj);
            }
        }

        return *this;
    };
    /////////////////    Move    ///////////////////////
    Grid(Grid&& grid)
    {
        N_x = grid.N_x;
        N_y = grid.N_y;
        m_data = grid.m_data;
        grid.m_data = nullptr;
    }
    Grid& operator=(Grid&& grid)
    {
        if(&grid == this)
            return *this;

        delete[] m_data;

        N_x = grid.N_x;
        N_y = grid.N_y;
        m_data = grid.m_data;
        grid.m_data = nullptr;

        return *this;
    }

    T& index(int ii, int jj) const;

    int getNx() const {return N_x;};
    int getNy() const {return N_y;};

};


template<class T>
T& Grid<T>::index(int ii, int jj) const
{
    assert(ii >= 0 && ii < N_x && jj >= 0 && jj < N_y && "Grid.index out of bounds");
    return m_data[jj * N_x + ii];
}



//***********************************************************
//       Mesh Data
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







//***********************************************************
//       OpenGL functions
//***********************************************************

/***************** loadDataToVBO  ******************
 * @brief Load vertex data into passed vbo.  Associated VAO
 *      must first be bound before calling this function.
 *
 *      Uses GL_STATIC_DRAW for vertices if drawStyle not passed.
******************************************************************///
template<class VertT>
void loadDataToVBO(unsigned int VBO, const std::vector<VertT>& vertices, GLenum drawStyle=GL_STATIC_DRAW)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(VertT), &vertices[0], drawStyle);
}


template<class ElementT>
void loadDataToEBO(unsigned int EBO, const std::vector<ElementT>& elements, GLenum drawStyle=GL_STATIC_DRAW)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size()*sizeof(ElementT), &elements[0], drawStyle);
}



/***************** SetVertexAttribs  ******************
 * @brief Set the vertex attributes for a particular type of vertex data.  The desired
 *      VAO must be bound before calling this.
 *
 * @tparam vertexType Pass in the vertex type to determine what attribute to set for the vertex buffer.
******************************************************************///
template <typename vertexType>
void SetVertexAttribs();



} // End namespace seng

#endif //SIM_ENGINE_COMMON_H
