#include "Engine.h"

int main()
{
    Engine engine;

    while (engine.IsOpen())
    {
        engine.Update();

        engine.Render();
    }


    return 0;
}