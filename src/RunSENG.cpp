#include <iostream>
#include "Engine/SimEngine.h"


using namespace seng;

int main()
{
    SimEngine engine{};

    if(engine.StartUp(800, 600))
    {
        engine.Run();
    }
    else
    {
        std::cout << "Failed to startup engine\n";
    }

    engine.ShutDown();
}
