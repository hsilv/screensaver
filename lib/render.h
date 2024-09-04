#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include <vector>
#include <fftw3.h>
#include "constants.h"

void renderBars(SDL_Renderer *renderer, fftwf_complex *fft_output, int numBars, std::vector<float> &smoothedMagnitudes);

#endif // RENDER_H