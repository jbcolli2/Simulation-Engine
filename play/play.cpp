//
// Created by jcollin2 on 7/13/22.
//

#include <iostream>

#include "Misc/Common.h"




using namespace seng;



template <typename T>
void print();

template<>
void print<Vert3x3x2f>()
{
    std::cout << "It worked!\n";
}


int main()
{
//    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return 0;
//    }


    print<Vert3x3x2f>();
    print<int>();
}