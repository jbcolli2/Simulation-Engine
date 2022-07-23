//
// Created by jcollin2 on 7/13/22.
//

#include <iostream>

#include "Misc/Util.h"





using namespace seng;

int main()
{
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 0;
    }

    std::string str = loadFile2String("../assets/shaders/PosNormUVVert.glsl");


}