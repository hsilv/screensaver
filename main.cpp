#include <iostream>
#include <limits>
#include <stdexcept>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <portaudio.h>
#include "window.h"
#include "render.h"
#include "audio.h"

void printUsage(const char *programName)
{
    std::cout << "Usage: " << programName << " [numBars]" << std::endl;
    std::cout << "  numBars: Optional. The number of bars to display. Must be a positive integer." << std::endl;
}

int main(int argc, char *argv[])
{
    int numBars = NUM_BARS;
    if (argc > 1)
    {
        try
        {
            int inputNumBars = std::stoi(argv[1]);
            if (inputNumBars > 0 && inputNumBars <= std::numeric_limits<int>::max())
            {
                numBars = inputNumBars;
            }
            else
            {
                std::cerr << "Error: The number of bars must be a positive integer." << std::endl;
                printUsage(argv[0]);
                return 1;
            }
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << "Error: Invalid argument. Please provide a valid integer." << std::endl;
            printUsage(argv[0]);
            return 1;
        }
        catch (const std::out_of_range &e)
        {
            std::cerr << "Error: The number provided is out of range." << std::endl;
            printUsage(argv[0]);
            return 1;
        }
    }

    SDL_Window *window = initWindow("Spectrum Analyzer", 800, 600);
    if (!window)
        return 1;

    SDL_Renderer *renderer = initRenderer(window);
    if (!renderer)
    {
        cleanup(window, nullptr);
        return 1;
    }

    if (Pa_Initialize() != paNoError)
    {
        std::cerr << "Error: Failed to initialize PortAudio." << std::endl;
        cleanup(window, renderer);
        return 1;
    }

    AudioData audioData;
    initAudio(audioData);

    PaStream *stream;
    if (Pa_OpenDefaultStream(&stream, 1, 0, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, audioCallback, &audioData) != paNoError)
    {
        std::cerr << "Error: Failed to open PortAudio stream." << std::endl;
        cleanupAudio(audioData);
        cleanup(window, renderer);
        Pa_Terminate();
        return 1;
    }

    if (Pa_StartStream(stream) != paNoError)
    {
        std::cerr << "Error: Failed to start PortAudio stream." << std::endl;
        Pa_CloseStream(stream);
        cleanupAudio(audioData);
        cleanup(window, renderer);
        Pa_Terminate();
        return 1;
    }

    bool quit = false;
    SDL_Event e;
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        renderBars(renderer, audioData.fft_output, numBars, audioData.smoothedMagnitudes);

        SDL_RenderPresent(renderer);
    }

    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    cleanupAudio(audioData);
    cleanup(window, renderer);

    return 0;
}