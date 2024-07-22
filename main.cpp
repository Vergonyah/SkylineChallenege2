#include "window.h"
#include "input.h"

int main()
{
    Window window(800, 600, "Terrain Renderer - Initial Setup");
    if (!window.init())
    {
        std::cout << "Failed to initialize window" << std::endl;
        return -1;
    }

    // Rendering loop
    while (!window.shouldClose())
    {
        processInput(window.getWindow());
        window.clear();
        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}