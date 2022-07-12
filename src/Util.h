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





namespace seng
{
/***************** loadFile2String  ******************
 * @brief Opens the file at @path and loads returns the contents as a string.
 *
 * @param path The full path of the file to be opened.
 *
 * @returns The contents of the file at path.
******************************************************************/
std::string loadFile2String(const char *path);



} // End namespace seng




#endif //SIM_ENGINE_UTIL_H
