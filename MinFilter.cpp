#include "Engine.h"

int main()
{
    Engine engine;

    while (engine.WindowOpen())
    {
        engine.Update();

        engine.Render();
    }


    return 0;
}