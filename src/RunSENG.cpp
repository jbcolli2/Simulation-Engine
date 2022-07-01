#include <iostream>
#include "SimEngine.h"

int main()
{
    SimEngine engine{};

    engine.Start(800, 600);
    engine.Run();
    engine.Stop();
}
