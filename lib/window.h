#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <vector>
#include "particle.h"

class Window
{
public:
    Window(const char *title, int width, int height, int numParticles, int refreshRate = 144, float speed = 1.0f);
    ~Window();

    void mainLoop();
    void setZoom(float newZoom);

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool isRunning;
    int refreshRate;
    float speed;
    float zoom;
    float viewCenterX, viewCenterY;
    float cameraDistance;
    float cameraAngleX, cameraAngleY;

    std::vector<Particle> particles;

    void initParticles(int numParticles);
    void updateParticles(float speed);
    void renderParticles();
};

#endif