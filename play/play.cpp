//
// Created by jcollin2 on 7/13/22.
//

#include <iostream>
#include <unordered_map>
#include <vector>

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
    std::vector<int> vec{};
    Derived(int a, int b)
    {
        vec = std::vector<int>{a, b};
        std::cout << "Create Derived...\n";
    }

    void print()
    {
        std::cout << "size = " << vec.size() << std::endl;
    }

    ~Derived()
    {
        std::cout << "Destroy Derived...\n";
    }
};





int main()
{
//    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return 0;
//    }

    std::unordered_map<std::string, std::unique_ptr<Derived>> map;

    std::unique_ptr<Derived> p{new Derived(1,1)};

    p.reset();

}