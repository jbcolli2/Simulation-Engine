//
// Created by jcollin2 on 7/13/22.
//

#include <iostream>
#include <array>

#include "Misc/Common.h"




using namespace seng;



class Comp
{
    int intVal{-1};

public:
    Comp(int ii)
    {
        intVal = ii;
        int jj = 8+intVal;
    }

    void print() {std::cout << intVal << std::endl; };
};


class Test
{
private:
    Comp* m_comp;

public:
    Test(int compii) : m_comp(new Comp(compii)) {};

    void print() {m_comp->print();};
};


int main()
{
//    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return 0;
//    }


    Test test(3);

    test.print();
}