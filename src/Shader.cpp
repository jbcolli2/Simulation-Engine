//
//  Shader.cpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 7/28/21.
//

#define GL_SILENCE_DEPRECATION


#include "Shader.hpp"

using namespace seng;

/***************** Shader Ctor  ******************
 * @brief - Loads in the shader source code to a string.
 *      - Compile the source code and link into a program.
******************************************************************///
Shader::Shader(std::string vsPath, std::string fsPath) :
    m_vertShaderPath(vsPath), m_fragShaderPath(fsPath)
{
    std::string vertShaderStr = loadFile2String(vsPath.c_str());
    std::string fragShaderStr = loadFile2String(fsPath.c_str());

    makeProgram(vertShaderStr, fragShaderStr);
}

Shader::Shader(std::string vsPath, std::string gsPath, std::string fsPath) :
        m_vertShaderPath(vsPath), m_fragShaderPath(fsPath), m_geomShaderPath(gsPath)
{
    std::string vertShaderStr = loadFile2String(vsPath.c_str());
    std::string fragShaderStr = loadFile2String(fsPath.c_str());
    std::string geomShaderStr = loadFile2String(gsPath.c_str());

    makeProgram(vertShaderStr, fragShaderStr, geomShaderStr);
}






/***************** makeProgram  ******************
 * @brief Take shader source code and compile each shader and link it together.
 *      Store the program handle in @m_program.
******************************************************************/
void Shader::makeProgram(const std::string& vertShaderSource,
                         const std::string& fragShaderSource,
                         const std::string& geomShaderSource)
{
    // Vertex Shader
    const char* vertSource = vertShaderSource.c_str();

    unsigned int vertShader;
    vertShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertShader, 1, &vertSource, NULL);
    glCompileShader(vertShader);

    // Check compile status
    int vertSuccess;
    char infoLog[512];
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &vertSuccess);
    if(!vertSuccess)
    {
        glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
        std::cout << "VertexShader::Compile::Fail  " << infoLog << std::endl;
    }
    
    
    // Geometry Shader
    unsigned int geomShader = 0;
    if(!geomShaderSource.empty())
    {
        const char* geomSource = geomShaderSource.c_str();

        geomShader = glCreateShader(GL_GEOMETRY_SHADER);

        glShaderSource(geomShader, 1, &geomSource, NULL);
        glCompileShader(geomShader);

        // Check compile status
        int geomSuccess;
        glGetShaderiv(geomShader, GL_COMPILE_STATUS, &geomSuccess);
        if(!geomSuccess)
        {
            glGetShaderInfoLog(geomShader, 512, NULL, infoLog);
            std::cout << "GeometryShader::Compile::Fail  " << infoLog << std::endl;
        }
    }
    



    // Fragment Shader
    const char* fragSource = fragShaderSource.c_str();

    unsigned int fragShader;
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragShader, 1, &fragSource, NULL);
    glCompileShader(fragShader);

    // Check compile status
    int fragSuccess;
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &fragSuccess);
    if(!fragSuccess)
    {
        glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
        std::cout << "FragmentShader::Compile::Fail  " << infoLog << std::endl;
    }


    m_program = glCreateProgram();
    glAttachShader(m_program, vertShader);
    if(!geomShaderSource.empty())
    {
        glAttachShader(m_program, geomShader);
    }
    glAttachShader(m_program, fragShader);
    glLinkProgram(m_program);
    int linkSuccess;
    glGetProgramiv(m_program, GL_LINK_STATUS, &linkSuccess);
    if(!linkSuccess)
    {
        glGetProgramInfoLog(m_program, 512, NULL, infoLog);
        std::cout << "ShaderProgram::Linker::Fail  " << infoLog << std::endl;
    }

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
}






/***************** getUniformLocation  ******************
 * @brief Returns the uniform location to be used in @glUniform methods to set a
 *      uniform in a shader program.
 *
 *      1. Tries to look up in hash table @m_uniforms.  If found returns location.
 *      2. If look up fails, the uses @glGetUniformLocation to find a location within the
 *          program for the uniform.
 *      3. Updates the hash table and returns location.
******************************************************************///
unsigned int Shader::getUniformLocation(const char* uniform)
{
    auto itUniform = m_uniforms.find(uniform);
    if(itUniform != m_uniforms.end())
    {
        return itUniform->second;
    }

    // This uniform has never been located in the program before
    unsigned int location = glGetUniformLocation(m_program, uniform);
    m_uniforms.insert({uniform, location});
    return location;
}