//
// Created by jcollin2 on 7/10/22.
//


#include "Util.h"



using namespace seng;



/***************** loadFile2String  ******************
 * @brief Opens the file at @path and loads returns the contents as a string.
******************************************************************/
std::string seng::loadFile2String(const char *path)
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