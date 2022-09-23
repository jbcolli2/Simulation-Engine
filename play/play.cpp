//
// Created by jcollin2 on 7/13/22.
//

#include <iostream>
#include <unordered_map>

//#include "Misc/Common.h"



class Move
{
private:
    float speed{1.f};

public:
    float& Speed() {return speed;};
};





int main()
{
//    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return 0;
//    }

    int test{-5};

    void print(std::string s)
    {
        std::cout << "s " << test << std::endl;
    }

    print("test is ");


}