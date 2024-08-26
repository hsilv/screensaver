#include "window.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <number_of_particles>" << std::endl;
        return 1;
    }

    int numParticles = std::atoi(argv[1]);
    Window window("SDL2 Window", 800, 600, numParticles);
    window.mainLoop();
    return 0;
}