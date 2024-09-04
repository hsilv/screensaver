#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "window.h"

/**
 * @brief Inicializa una ventana SDL.
 *
 * Esta función inicializa SDL y SDL_ttf, y crea una ventana SDL con el título, ancho y alto especificados.
 *
 * @param title Título de la ventana.
 * @param width Ancho de la ventana.
 * @param height Alto de la ventana.
 * @return Puntero a la ventana SDL creada, o nullptr si ocurre un error.
 */
SDL_Window *initWindow(const char *title, int width, int height)
{
    // Inicializar SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    // Inicializar SDL_ttf
    if (TTF_Init() == -1)
    {
        std::cerr << "TTF_Init: " << TTF_GetError() << std::endl;
        return nullptr;
    }

    // Crear la ventana SDL
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
 * Esta función crea un renderer SDL para la ventana especificada.
 *
 * @param window Puntero a la ventana SDL.
 * @return Puntero al renderer SDL creado, o nullptr si ocurre un error.
 */
SDL_Renderer *initRenderer(SDL_Window *window)
{
    // Crear el renderer SDL
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }
    return renderer;
}

/**
 * @brief Limpia y cierra SDL y SDL_ttf.
 *
 * Esta función destruye el renderer y la ventana SDL, y cierra SDL y SDL_ttf.
 *
 * @param window Puntero a la ventana SDL.
 * @param renderer Puntero al renderer SDL.
 */
void cleanup(SDL_Window *window, SDL_Renderer *renderer)
{
    // Destruir el renderer SDL
    SDL_DestroyRenderer(renderer);
    // Destruir la ventana SDL
    SDL_DestroyWindow(window);
    // Cerrar SDL_ttf
    TTF_Quit();
    // Cerrar SDL
    SDL_Quit();
}