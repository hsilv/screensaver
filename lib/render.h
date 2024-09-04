#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include <vector>
#include <complex>
#include "constants.h"

void renderBars(SDL_Renderer *renderer, const std::vector<std::complex<float>> &fft_output, int numBars, std::vector<float> &smoothedMagnitudes);
#endif