#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "window.h"

Window::Window(const char *title, int width, int height, int numParticles, int refreshRate, float speed)
    : refreshRate(refreshRate), speed(speed), zoom(1.0f), viewCenterX(width / 2.0f), viewCenterY(height / 2.0f),
      cameraDistance(150.0f), cameraAngleX(0.0f), cameraAngleY(0.0f)
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
    updateParticles(speed);
}

Window::~Window()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Window::initParticles(int numParticles)
{
    particles.resize(numParticles);
    float angleStep = 0.001f;
    float radiusStep = 0.1f;
    float angle = 0.0f;
    float radius = 0.0f;

    for (int i = 0; i < numParticles; ++i)
    {
        auto &particle = particles[i];

        if (i % 2 == 0)
        {
            particle.x = radius * cos(angle);
            particle.y = radius * sin(angle);
        }
        else
        {
            particle.x = radius * cos(angle + M_PI);
            particle.y = radius * sin(angle + M_PI);
        }

        particle.z = 0.0f;
        particle.vx = 0.0f;
        particle.vy = 0.0f;
        particle.vz = 0.0f;

        particle.r = rand() % 256;
        particle.g = rand() % 256;
        particle.b = rand() % 256;

        angle += angleStep;
        radius += radiusStep;
    }
}

void Window::updateParticles(float speed)
{
    float dt = 0.5f;
    float sigma = 10.0f;
    float rho = 28.0f;
    float beta = 8.0f / 3.0f;
    float maxSpeed = 60.0f;

    for (auto &particle : particles)
    {
        particle.update(dt, sigma, rho, beta, speed, maxSpeed);
    }
}

void Window::renderParticles()
{

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    glm::vec3 cameraPos = glm::vec3(
        cameraDistance * sin(glm::radians(cameraAngleY)) * cos(glm::radians(cameraAngleX)),
        cameraDistance * sin(glm::radians(cameraAngleX)),
        cameraDistance * cos(glm::radians(cameraAngleY)) * cos(glm::radians(cameraAngleX)));

    glm::mat4 view = glm::lookAt(
        cameraPos,
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 25);
    SDL_RenderFillRect(renderer, nullptr);

    for (const auto &particle : particles)
    {
        glm::vec4 pos(particle.x, particle.y, particle.z, 1.0f);
        glm::vec4 transformedPos = projection * view * pos;

        int renderX = static_cast<int>((transformedPos.x / transformedPos.w) * 400 + 400);
        int renderY = static_cast<int>((transformedPos.y / transformedPos.w) * 300 + 300);

        SDL_SetRenderDrawColor(renderer, particle.r, particle.g, particle.b, 255);
        SDL_RenderDrawPoint(renderer, renderX, renderY);
    }

    SDL_RenderPresent(renderer);
}

void Window::mainLoop()
{
    SDL_Event e;
    Uint32 frameDelay = 1000 / refreshRate;
    Uint32 frameStart, frameTime;
    int frameCount = 0;
    Uint32 lastTime = SDL_GetTicks();
    Uint32 startTime = SDL_GetTicks();

    while (isRunning)
    {
        frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                isRunning = false;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                std::cout << "Key pressed: " << SDL_GetKeyName(e.key.keysym.sym) << std::endl;
                switch (e.key.keysym.sym)
                {
                case SDLK_PLUS:
                case SDLK_EQUALS:
                case SDLK_KP_PLUS:
                    cameraDistance -= 10.0f;
                    break;
                case SDLK_MINUS:
                case SDLK_KP_MINUS:
                    cameraDistance += 10.0f;
                    break;
                case SDLK_UP:
                    cameraAngleX -= 5.0f;
                    break;
                case SDLK_DOWN:
                    cameraAngleX += 5.0f;
                    break;
                case SDLK_LEFT:
                    cameraAngleY -= 5.0f;
                    break;
                case SDLK_RIGHT:
                    cameraAngleY += 5.0f;
                    break;
                default:
                    break;
                }
            }
        }

        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - startTime >= 4000)
        {
            cameraAngleY += 0.4f;
        }

        if (currentTime - startTime >= 60000)
        {
            initParticles(particles.size());
            startTime = currentTime;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        updateParticles(speed);
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

void Window::setZoom(float newZoom)
{
    zoom = newZoom;
}