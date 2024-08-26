#include "window.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <number_of_particles> <speed>" << std::endl;
        return 1;
    }

    int numParticles = std::atoi(argv[1]);
    float speed = std::atof(argv[2]);
    Window window("SDL2 Window", 800, 600, numParticles, 144, speed);
    window.mainLoop();
    return 0;
}