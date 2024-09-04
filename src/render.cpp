#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include <iostream>
#include "render.h"

void renderBars(SDL_Renderer *renderer, fftwf_complex *fft_output, int numBars, std::vector<float> &smoothedMagnitudes)
{
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);

    int barWidth = width / numBars;
    float freqPerBin = SAMPLE_RATE / static_cast<float>(FRAMES_PER_BUFFER);

    for (int i = 0; i < numBars; ++i)
    {
        float startFreq = MIN_FREQ * std::pow(MAX_FREQ / MIN_FREQ, static_cast<float>(i) / numBars);
        float endFreq = MIN_FREQ * std::pow(MAX_FREQ / MIN_FREQ, static_cast<float>(i + 1) / numBars);
        int startBin = static_cast<int>(startFreq / freqPerBin);
        int endBin = static_cast<int>(endFreq / freqPerBin);

        float magnitude = 0.0f;
        if (endBin <= startBin)
        {
            endBin = startBin + 1; // Asegurar al menos un bin
        }

        for (int bin = startBin; bin < endBin; ++bin)
        {
            if (bin < FRAMES_PER_BUFFER / 2)
            {
                float real = fft_output[bin][0];
                float imag = fft_output[bin][1];
                if (std::isnan(real) || std::isnan(imag) || std::isinf(real) || std::isinf(imag))
                {
                    std::cerr << "Invalid FFT output at bin " << bin << ": real = " << real << ", imag = " << imag << std::endl;
                }
                magnitude += std::sqrt(real * real + imag * imag);
            }
        }

        magnitude /= (endBin - startBin); // Average magnitude over the range

        if (std::isnan(magnitude) || std::isinf(magnitude))
        {
            magnitude = 0.0f; // Reset invalid magnitude to 0
        }

        smoothedMagnitudes[i] = SMOOTHING_FACTOR * smoothedMagnitudes[i] + (1.0f - SMOOTHING_FACTOR) * magnitude;

        int barHeight = static_cast<int>((smoothedMagnitudes[i] / 100.0) * height);
        SDL_Rect bar = {i * barWidth, height - barHeight, barWidth - 2, barHeight};
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &bar);
    }
}