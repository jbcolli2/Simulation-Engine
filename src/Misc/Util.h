//
// Created by jcollin2 on 7/3/22.
//

#ifndef SIM_ENGINE_UTIL_H
#define SIM_ENGINE_UTIL_H


#include <cstdint>
#include <bitset>
#include <typeinfo>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#include "glad/glad.h"
#include <glm/glm.hpp>




namespace seng
{


//***********************************************************
//       Component Information
//***********************************************************
constexpr int MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;




/***************** loadFile2String  ******************
 * @brief Opens the file at @path and loads returns the contents as a string.
 *
 * @param path The full path of the file to be opened.
 *
 * @returns The contents of the file at path.
******************************************************************/
std::string loadFile2String(const char *path);






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

} // End namespace seng





//***********************************************************
//       OpenGL functions
//***********************************************************

/***************** loadDataToVBO  ******************
 * @brief Generate a VBO and load vertex data into it.  Associated VAO
 *      must first be bound before calling this function.
 *
 *      Uses GL_STATIC_DRAW for vertices.
 *      TODO: Should be changed for free meshes.  Pass in parameter or define different function.
******************************************************************///
template<class VertT>
unsigned int loadDataToVBO(std::vector<VertT> vertices)
{
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(VertT), &vertices[0], GL_STATIC_DRAW);


    return VBO;
}

#endif //SIM_ENGINE_UTIL_H
