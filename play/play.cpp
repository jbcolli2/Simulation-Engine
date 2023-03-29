//
// Created by jcollin2 on 7/13/22.
//

#include <iostream>
#include <unordered_map>
#include <vector>

//#include "Misc/Common.h"

class Shader
{
public:
    int m_id{0};
    Shader(int id) : m_id(id){};
    void Print()
    {
        std::cout << "id = " << m_id << std::endl;
    }
};

class Base
{
private:
    virtual void print()
    {
        std::cout << "Base print\n";
    };

public:
    static Shader* m_shader;
    void SetShader(const Shader& shader)
    {
        if(m_shader != nullptr)
        {
            std::cout << "WARNING: Trying to set Shader twice on a Material!!!\n";
            return;
        }
        m_shader = &const_cast<Shader&>(shader);
    }

    void show()
    {
        std::cout << "Print Base\n";
    }
};
Shader* Base::m_shader = nullptr;

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


Base* MakeBase()
{
    Base* base = new Derived(4,4);

    return base;
}


int main()
{
//    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return 0;
//    }

    Base* base = MakeBase();

    base->show();

}