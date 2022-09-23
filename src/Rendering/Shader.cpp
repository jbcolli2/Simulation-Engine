//
//  Shader.cpp
//  LearnOpenGL
//
//  Created by Jeb Collins on 7/28/21.
//



#include "Shader.h"
#include "Components/Lights.h"

#include "Rendering/Material.h"

using namespace seng;


//***********************************************************
//       PointLight Shader Implementation
//***********************************************************
const std::string PointLightShader::m_flagStr = "usePointLight";
const std::string PointLightShader::m_positionStr = "pointLight.position";
const std::string PointLightShader::m_colorStr = "pointLight.color";
const std::string PointLightShader::m_diffuseIntensityStr = "pointLight.diffIntensity";
const std::string PointLightShader::m_specIntensityStr = "pointLight.specIntensity";


void PointLightShader::SetUniform(const PointLight& light)
{
    m_shader.setUniform1i(m_flagStr, 1);
    m_shader.setUniform3f(m_positionStr, light.m_position.x, light.m_position.y, light.m_position.z);
    m_shader.setUniform3f(m_colorStr, light.m_color.r, light.m_color.g, light.m_color.b);
    m_shader.setUniform1f(m_diffuseIntensityStr, light.m_diffuseIntensity);
    m_shader.setUniform1f(m_specIntensityStr, light.m_specularIntensity);
}




//***********************************************************
//       Direction Light Shader Code
//***********************************************************
const std::string DirLightShader::m_flagStr = "useDirLight";
const std::string DirLightShader::m_directionStr = "dirLight.direction";
const std::string DirLightShader::m_colorStr = "dirLight.color";
const std::string DirLightShader::m_diffuseIntensityStr = "dirLight.diffIntensity";
const std::string DirLightShader::m_specIntensityStr = "dirLight.specIntensity";


void DirLightShader::SetUniform(const DirLight& light)
{
    m_shader.setUniform1i(m_flagStr, 1);
    m_shader.setUniform3f(m_directionStr, light.m_direction.x, light.m_direction.y, light.m_direction.z);
    m_shader.setUniform3f(m_colorStr, light.m_color.r, light.m_color.g, light.m_color.b);
    m_shader.setUniform1f(m_diffuseIntensityStr, light.m_diffuseIntensity);
    m_shader.setUniform1f(m_specIntensityStr, light.m_specularIntensity);
}




//***********************************************************
//       Solid Material Shader Code
//***********************************************************
const std::string SolidMatShader::m_flagStr = "useSolidMaterial";
const std::string SolidMatShader::m_diffColorStr = "solidMat.diffuse";
const std::string SolidMatShader::m_specColorStr = "solidMat.specular";
const std::string SolidMatShader::m_roughnessStr = "solidMat.roughness";


void SolidMatShader::SetUniform(const SolidMaterial& material)
{
    m_shader.setUniform1ui(m_flagStr, 1);
    m_shader.setUniform3f(m_diffColorStr, material.m_diffuse.x, material.m_diffuse.y, material.m_diffuse.z);
    m_shader.setUniform3f(m_specColorStr, material.m_specular.r, material.m_specular.g, material.m_specular.b);
    m_shader.setUniform1f(m_roughnessStr, material.m_roughness);
}



//***********************************************************
//       Texture Material Shader Code
//***********************************************************
const std::string TextureMatShader::m_flagStr = "useTextureMaterial";
const std::string TextureMatShader::m_diffTexUnitStr = "textureMat.diffuse";
const std::string TextureMatShader::m_specColorStr = "textureMat.specular";
const std::string TextureMatShader::m_roughnessStr = "textureMat.roughness";


void TextureMatShader::SetUniform(const TextureMaterial& material)
{
    m_shader.setUniform1ui(m_flagStr, 1);
    m_shader.setUniform1i(m_diffTexUnitStr, material.m_diffTexture.m_texUnit);
    m_shader.setUniform3f(m_specColorStr, material.m_specColor.r, material.m_specColor.g, material.m_specColor.b);
    m_shader.setUniform1f(m_roughnessStr, material.m_roughness);
}



//***********************************************************
//       Shader Code
//***********************************************************
const std::string Shader::m_ambientStr = "ambientIntensity";
const std::string Shader::m_camPositionStr = "camPosition";


void Shader::SetAmbientUniform(float ambient)
{
    setUniform1f(m_ambientStr, ambient);
}

void Shader::SetCamPositionUniform(glm::vec3 camPosition)
{
    setUniform3f(m_camPositionStr, camPosition.x, camPosition.y, camPosition.z);
}




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
    glDetachShader(m_program, vertShader);
    glDeleteShader(vertShader);
    glDetachShader(m_program, fragShader);
    glDeleteShader(fragShader);

    if(!geomShaderSource.empty())
    {
        glDetachShader(m_program, geomShader);
        glDeleteShader(geomShader);
    }
}


void Shader::deepCopy(Shader& other)
{
    m_program = other.m_program;
    m_vertShaderPath = other.m_vertShaderPath;
    m_fragShaderPath = other.m_fragShaderPath;
    m_geomShaderPath = other.m_geomShaderPath;

    m_beingUsed = other.m_beingUsed;

    m_uniforms = other.m_uniforms;
}

Shader::Shader(Shader&& other)
{
    deepCopy(other);
    other.m_program = 0;
    other.m_uniforms.clear();
    other.m_vertShaderPath = "";
    other.m_geomShaderPath = "";
    other.m_fragShaderPath = "";
    other.m_beingUsed = false;
}

Shader& Shader::operator=(Shader&& other)
{
    // Check self assignment
    if(this == &other)
    {
        return *this;
    }

    deepCopy(other);
    other.m_program = 0;
    other.m_uniforms.clear();
    other.m_vertShaderPath = "";
    other.m_geomShaderPath = "";
    other.m_fragShaderPath = "";
    other.m_beingUsed = false;

    return *this;
}

Shader::~Shader()
{
    glDeleteShader(m_program);
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