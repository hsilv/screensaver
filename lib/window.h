#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <vector>
#include "particle.h"

class Window
{
public:
    Window(const char *title, int width, int height, int numParticles, int refreshRate = 144);
    ~Window();
    void mainLoop();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool isRunning;
    int refreshRate;
    std::vector<Particle> particles;

    void initParticles(int numParticles);
    void updateParticles();
    void renderParticles();
};

#endif // WINDOW_H