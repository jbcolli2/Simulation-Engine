//
// Created by jcollin2 on 7/13/22.
//

#include <iostream>
#include <unordered_map>

//#include "Misc/Common.h"



class Base
{
protected:
    float speed{1.f};

public:
    float Speed() {return speed;};
};


class Derived : public Base
{
public:
    void Print()  {std::cout << "Derived speed";};
};

void print(Base*&& base)
{
    std::cout << "x = " << base->Speed() << std::endl;
}



int main()
{
//    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return 0;
//    }

    Base* pB = new Base();


    print(pB);




}