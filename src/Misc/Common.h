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
#include <unordered_map>

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


/***************** Matrix2VectorIdx  ******************
 * @brief Convert matrix indices into vector indices for the cloth grid.  Vector
 *      index starts at bottom left and fills in row first.
 *
 * @param ii,jj Matrix indices.  ii is x and jj is y.
 *
 * @returns Vector index of same spot on grid.
******************************************************************///
inline int Matrix2VectorIdx(int ii, int jj, int Nx)
{
    return jj*Nx + ii;
}

inline void Vector2MatrixIdx(int vert, int Nx, int& ii, int& jj)
{
    ii = vert % Nx;
    jj = vert/Nx;
}










//***********************************************************
//       OpenGL functions
//***********************************************************





unsigned int GenAndLoadTBO(const std::string& filename, bool sRGB = false);

void PrintFramebufferStatus(unsigned int fbStatus);




} // End namespace seng

#endif //SIM_ENGINE_COMMON_H
