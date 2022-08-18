//
// Created by jcollin2 on 7/10/22.
//


#include "Common.h"





namespace seng
{


/***************** loadFile2String  ******************
 * @brief Opens the file at @path and loads returns the contents as a string.
******************************************************************/
std::string seng::loadFile2String(const char* path)
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


} // end namespace seng

