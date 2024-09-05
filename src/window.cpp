#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "window.h"

/**
 * @brief Inicializa una ventana SDL.
 *
 * Esta función inicializa el subsistema de video de SDL y SDL_ttf, y crea una ventana SDL con el título, ancho y alto especificados.
 *
 * @param title El título de la ventana.
 * @param width El ancho de la ventana.
 * @param height El alto de la ventana.
 * @return Un puntero a la ventana SDL creada, o nullptr si ocurre un error.
 */
SDL_Window *initWindow(const char *title, int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    if (TTF_Init() == -1)
    {
        std::cerr << "TTF_Init: " << TTF_GetError() << std::endl;
        return nullptr;
    }

    SDL_Window *window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    return window;
}

/**
 * @brief Inicializa un renderer SDL.
 *
 * Esta función crea un renderer SDL acelerado para la ventana especificada.
 *
 * @param window Un puntero a la ventana SDL.
 * @return Un puntero al renderer SDL creado, o nullptr si ocurre un error.
 */
SDL_Renderer *initRenderer(SDL_Window *window)
{
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }
    return renderer;
}

/**
 * @brief Limpia los recursos de SDL.
 *
 * Esta función destruye el renderer y la ventana SDL, y cierra los subsistemas de SDL_ttf y SDL.
 *
 * @param window Un puntero a la ventana SDL.
 * @param renderer Un puntero al renderer SDL.
 */
void cleanup(SDL_Window *window, SDL_Renderer *renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}