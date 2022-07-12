//
//  Shader.hpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 7/28/21.
//

#ifndef Shader_h
#define Shader_h

#include "Util.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <OpenGL/gl3.h>

#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <unordered_map>

namespace seng
{
class Shader
{
private:
    // Handle to the shader program
    unsigned int m_program{0};

    // Path for the various shader source code
    std::string m_vertShaderPath{""}, m_fragShaderPath{""}, m_geomShaderPath{""};

    // flag to store if shader is currently being used
    bool m_beingUsed{false};

    // hash table for location of uniforms
    std::unordered_map<const char *, unsigned int> m_uniforms{};

public:
    // Default Ctor
    Shader()
    {};
    /***************** Shader Ctor  ******************
     * @brief - Loads in the shader source code to a string.
     *      - Compile the source code and link into a program.
     * 
     * @param vsPath Full path to vertex shader file
     * @param gsPath Full path to geometry shader file
     * @param fsPath Full path to fragment shader file
    ******************************************************************///

    // Ctor with vertex and frag shaders
    Shader(std::string vsPath, std::string fsPath);

    // Ctor with vertex, geometry and frag shaders
    Shader(std::string vsPath, std::string gsPath, std::string fsPath);


    void startProgram()
    {
        glUseProgram(m_program);
        m_beingUsed = true;
    };

    void stopProgram()
    {
        glUseProgram(0);
        m_beingUsed = false;
    };

    // Returns true if program is being used currently
    bool InUse()
    { return m_beingUsed; };


    //***********************************************************
    //       Setting uniforms
    //***********************************************************
    void setUniform1f(const std::string &uniformName, float f)
    {
        if (auto loc = getUniformLocation(uniformName.c_str()); loc != -1)
            glUniform1f(loc, f);
    }

    void setUniform4f(const std::string &uniformName, float r, float g, float b, float a)
    {
        if (auto loc = getUniformLocation(uniformName.c_str()); loc != -1)
            glUniform4f(loc, r, g, b, a);
    }

    void setUniform3f(const std::string &uniformName, float r, float g, float b)
    {
        if (auto loc = getUniformLocation(uniformName.c_str()); loc != -1)
            glUniform3f(loc, r, g, b);
    }

    void setUniform1i(const std::string &uniformName, int i)
    {
        if (auto loc = getUniformLocation(uniformName.c_str()); loc != -1)
            glUniform1i(loc, i);
    }

    void setUniform1ui(const std::string &uniformName, unsigned int i)
    {
        if (auto loc = getUniformLocation(uniformName.c_str()); loc != -1)
            glUniform1ui(loc, i);
    }

    void setUniformTex(const std::string &uniformName, int i)
    {
        setUniform1i(uniformName, i);
    }


    void setUniformMatrix4f(const std::string &uniformName, glm::mat4 mat)
    {
        auto loc = getUniformLocation(uniformName.c_str());
        if (loc != -1)
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
    }

    void bindUniformBlock(const std::string &uniformBlockName, int bindPoint)
    {
        glUniformBlockBinding(m_program, bindPoint, glGetUniformBlockIndex(m_program, uniformBlockName.c_str()));
    }


private:
    /***************** makeProgram  ******************
     * @brief Take shader source code and compile each shader and link it together.
     *      Store the program handle in @m_program.
     *
     * @param vertShaderSource source code for the vertex shader
     * @param fragShaderSource source code for the fragment shader
     * @param geomShaderSource (Optional) source code for the geometry shader
    ******************************************************************/
    void makeProgram(const std::string &vertShaderSource,
                     const std::string &fragShaderSource,
                     const std::string &geomShaderSource = "");




    /***************** getUniformLocation  ******************
     * @brief Returns the uniform location to be used in @glUniform methods to set a
     *      uniform in a shader program.
     *
     * @param uniform the name of the uniform
     *
     * @returns int handle to location of uniform in the program.  If uniform is not used in the
     *      program, returns -1, same as @glGetUniformLocation
    ******************************************************************///
    unsigned int getUniformLocation(const char *uniform);

};  // end class Shader



} // end namespace pecs

#endif /* Shader_h */
