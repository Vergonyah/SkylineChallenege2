#include "window.h"
#include "input.h"
#include "terrain.h"
#include <iostream>

int main()
{
    Window window(800, 600, "Terrain Renderer");
    if (!window.init())
    {
        std::cout << "Failed to initialize window" << std::endl;
        return -1;
    }

    Terrain terrain(100);  // Create a 100x100 grid
    terrain.generate();

    // Rendering loop
    while (!window.shouldClose())
    {
        processInput(window.getWindow());
        window.clear();
        
        terrain.render();  // Render the terrain

        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}