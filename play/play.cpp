//
// Created by jcollin2 on 7/13/22.
//

#include <iostream>
#include <unordered_map>

//#include "Misc/Common.h"



class Base
{
private:
    virtual void print()
    {
        std::cout << "Base print\n";
    };
};


class Derived : public Base
{
public:
    void print() override
    {
        std::cout << "Make private method public, hah\n";
    };
};





int main()
{
//    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return 0;
//    }

    Derived* b = new Derived();
    b->print();


}