//
// Created by jcollin2 on 7/13/22.
//

#include <iostream>
#include <unordered_map>

//#include "Misc/Common.h"



class Move
{
protected:
    float speed{1.f};

public:
    float& Speed() {return speed;};
    void virtual Print() {std::cout << "Speed = " << speed << std::endl;}
};


class D : public Move
{
public:
    void Print() override {std::cout << "Derived speed";};
};





int main()
{
//    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return 0;
//    }

    D derived{};
    std::cout << "Speed is " << derived.Speed() << std::endl;
    derived.Print();


}