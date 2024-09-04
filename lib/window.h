#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>

SDL_Window *initWindow(const char *title, int width, int height);
SDL_Renderer *initRenderer(SDL_Window *window);
void cleanup(SDL_Window *window, SDL_Renderer *renderer);

#endif // WINDOW_H