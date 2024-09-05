#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include <iostream>
#include "render.h"

/**
 * @brief Genera un color pseudoaleatorio basado en el índice.
 *
 * @param index Índice de la barra.
 * @return SDL_Color Color generado.
 */
SDL_Color generateColor(int index)
{
    // Generar colores pseudoaleatorios basados en el índice
    Uint8 r = (index * 97) % 256;
    Uint8 g = (index * 53) % 256;
    Uint8 b = (index * 29) % 256;
    return {r, g, b, 255};
}

/**
 * @brief Renderiza barras de un espectro de audio en un renderer SDL.
 *
 * @param renderer Puntero al renderer SDL.
 * @param fft_output Vector de salida de la FFT.
 * @param numBars Número de barras a renderizar.
 * @param smoothedMagnitudes Vector de magnitudes suavizadas.
 */
void renderBars(SDL_Renderer *renderer, const std::vector<std::complex<float>> &fft_output, int numBars, std::vector<float> &smoothedMagnitudes)
{
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);

    if (fft_output.size() < numBars || smoothedMagnitudes.size() < numBars)
    {
        std::cerr << "Error: fft_output o smoothedMagnitudes no tienen el tamaño adecuado." << std::endl;
        return;
    }

    float barWidth = static_cast<float>(width) / numBars;
    for (int i = 0; i < numBars; ++i)
    {
        float magnitude = std::abs(fft_output[i]);
        smoothedMagnitudes[i] = SMOOTHING_FACTOR * smoothedMagnitudes[i] + (1.0f - SMOOTHING_FACTOR) * magnitude;

        float barHeight = smoothedMagnitudes[i] * height;
        SDL_Rect bar = {static_cast<int>(i * barWidth), height - static_cast<int>(barHeight), static_cast<int>(barWidth), static_cast<int>(barHeight)};

        SDL_Color color = generateColor(i);
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &bar);
    }
}