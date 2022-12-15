//
// Created by jcollin2 on 7/10/22.
//


#include "Common.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"



namespace seng
{


/***************** loadFile2String  ******************
 * @brief Opens the file at @path and loads returns the contents as a string.
******************************************************************/
std::string loadFile2String(const char* path)
{
    std::ifstream file;
    try {
        file.open(path, std::ifstream::in);
        file.exceptions(std::ifstream::failbit);
        std::stringstream fileStream;
        fileStream << file.rdbuf();

        file.close();

        return fileStream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "Error reading file at " << path << std::endl;
        file.close();
        return std::string("");
    }
}


//***********************************************************
//       OpenGL functions
//***********************************************************
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

template<>
void SetVertexAttribs<Vert3x2f>()
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert3x2f), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert3x2f), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);



};


unsigned int GenAndLoadTBO(const std::string& path, bool sRGB)
{
    unsigned int texID = 0;
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    unsigned int textureFormat, internalFormat;
    if(nrChannels == 3)
    {
        textureFormat = GL_RGB;
        internalFormat = sRGB ? GL_SRGB : GL_RGB;
    }
    else if(nrChannels == 4)
    {
        textureFormat = GL_RGBA;
        internalFormat = sRGB ? GL_SRGB_ALPHA : GL_RGBA;
    }
    else if(nrChannels == 1)
    {
        textureFormat = GL_RED;
        internalFormat = GL_RED;
    }
    else
    {
        std::cout << "Number of channels in image is not 3 or 4, it is " << nrChannels << "\n";
        return 0;
    }

    if(data)
    {
        glGenTextures(1, &texID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, textureFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failure to load texture " << path << std::endl;
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    return texID;

}


} // end namespace seng

