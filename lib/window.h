#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>

class Window
{
public:
    Window(const char *title, int width, int height, int refreshRate = 144);
    ~Window();
    void mainLoop();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool isRunning;
    int refreshRate;
};

#endif