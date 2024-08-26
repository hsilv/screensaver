#include "window.h"
#include <iostream>
#include <string>

Window::Window(const char *title, int width, int height, int refreshRate)
    : refreshRate(refreshRate)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        isRunning = false;
        return;
    }

    window = SDL_CreateWindow(title, 100, 100, width, height, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        isRunning = false;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
    {
        SDL_DestroyWindow(window);
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        isRunning = false;
        return;
    }

    isRunning = true;
}

Window::~Window()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Window::mainLoop()
{
    SDL_Event e;
    Uint32 frameDelay = 1000 / refreshRate;
    Uint32 frameStart, frameTime;
    int frameCount = 0;
    Uint32 lastTime = SDL_GetTicks();

    while (isRunning)
    {
        frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                isRunning = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }

        frameCount++;
        if (SDL_GetTicks() - lastTime >= 1000)
        {
            std::string title = "FPS: " + std::to_string(frameCount);
            SDL_SetWindowTitle(window, title.c_str());
            frameCount = 0;
            lastTime = SDL_GetTicks();
        }
    }
}