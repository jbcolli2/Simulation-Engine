//
// Created by jcollin2 on 7/13/22.
//

#include <iostream>
#include <unordered_map>

//#include "Misc/Common.h"









int main()
{
//    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return 0;
//    }


    std::unordered_map<int, std::string> cache = {{0, "hello"}};
    std::cout << cache[0] << std::endl;
    cache.insert({4, "four"});
    std::cout << cache[4] << std::endl;
//    cache[3] = "three";

    auto iter = cache.find(3);
    if(iter != cache.end())
    {
        std::cout << iter->second << std::endl;
    }

    cache[3] = "3";
    std::cout << 2399%50<< std::endl;

}