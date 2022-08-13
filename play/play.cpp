//
// Created by jcollin2 on 7/13/22.
//

#include <iostream>
#include <array>

#include "Misc/Common.h"




using namespace seng;







int main()
{
//    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return 0;
//    }


    Grid<std::vector<std::array<int, 2>>> grid{5,5};

    grid.index(1,1).push_back(std::array<int,2>{2,3});

    std::cout << grid.index(1,1)[0][0] << " | " << grid.index(1,1)[0][1] << "\n";

}