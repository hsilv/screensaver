#include "window.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

Window::Window(const char *title, int width, int height, int numParticles, int refreshRate)
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
    initParticles(numParticles);
}

Window::~Window()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Window::initParticles(int numParticles)
{
    std::srand(std::time(0));
    particles.clear();
    for (int i = 0; i < numParticles; ++i)
    {
        float x = std::rand() % 800;
        float y = std::rand() % 600;
        float z = std::rand() % 100;
        float vx = 0.0f;
        float vy = 0.0f;
        float vz = 0.0f;
        float speed = static_cast<float>(std::rand() % 100) / 100.0f;
        particles.emplace_back(x, y, z, vx, vy, vz, speed);
    }
    std::cout << "Initialized " << particles.size() << " particles." << std::endl;
}

void Window::updateParticles()
{

    float dt = 0.01f;
    float sigma = 10.0f;
    float rho = 28.0f;
    float beta = 8.0f / 3.0f;

    for (auto &particle : particles)
    {
        particle.update(dt, sigma, rho, beta);
        if (particle.x < 0 || particle.x >= 800)
            particle.vx = -particle.vx;
        if (particle.y < 0 || particle.y >= 600)
            particle.vy = -particle.vy;

        std::cout << "Particle x: " << particle.x << " y: " << particle.y << " vx: " << particle.vx << " vy: " << particle.vy << std::endl;
    }
}

void Window::renderParticles()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (const auto &particle : particles)
    {
        std::cout << "Rendering particle at (" << particle.x << ", " << particle.y << ")" << std::endl;
        SDL_RenderDrawPoint(renderer, static_cast<int>(particle.x), static_cast<int>(particle.y));
    }
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

        updateParticles();
        renderParticles();

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