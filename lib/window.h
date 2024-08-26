#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>

class Window
{
public:
    Window(const char *title, int width, int height);
    ~Window();
    void mainLoop();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool isRunning;
};

#endif // WINDOW_H