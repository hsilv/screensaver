#include <iostream>
#include <limits>
#include <stdexcept>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <portaudio.h>
#include <string>
#include <chrono>
#include <thread>
#include "window.h"
#include "render.h"
#include "audio.h"
#include "filters.h"

/**
 * @brief Imprime el uso del programa.
 *
 * @param programName Nombre del programa.
 */
void printUsage(const char *programName)
{
    std::cout << "Usage: " << programName << " <number_of_bars>" << std::endl;
}

/**
 * @brief Verifica si una cadena es un número.
 *
 * @param str Cadena a verificar.
 * @return true si la cadena es un número, false en caso contrario.
 */
bool isNumber(const std::string &str)
{
    for (char const &c : str)
    {
        if (!std::isdigit(c))
            return false;
    }
    return true;
}

/**
 * @brief Función principal del programa.
 *
 * @param argc Número de argumentos de línea de comandos.
 * @param argv Array de argumentos de línea de comandos.
 * @return int Código de retorno del programa.
 */
int main(int argc, char *argv[])
{
    int numBars = NUM_BARS;

    // Verificar que se haya proporcionado al menos un argumento
    if (argc > 1)
    {
        std::string arg1 = argv[1];

        // Verificar que el argumento sea un número
        if (isNumber(arg1))
        {
            try
            {
                numBars = std::stoi(arg1);
                if (numBars <= 0)
                {
                    std::cerr << "Number of bars must be a positive integer." << std::endl;
                    return 1;
                }
            }
            catch (const std::invalid_argument &e)
            {
                std::cerr << "Invalid argument: " << e.what() << std::endl;
                printUsage(argv[0]);
                return 1;
            }
            catch (const std::out_of_range &e)
            {
                std::cerr << "Argument out of range: " << e.what() << std::endl;
                printUsage(argv[0]);
                return 1;
            }
        }
        else
        {
            std::cerr << "The argument must be a positive integer." << std::endl;
            printUsage(argv[0]);
            return 1;
        }
    }
    else
    {
        std::cerr << "No arguments provided." << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    // Inicializar la ventana SDL
    SDL_Window *window = initWindow("Spectrum Analyzer", 800, 600);
    if (!window)
        return 1;

    // Inicializar el renderer SDL
    SDL_Renderer *renderer = initRenderer(window);
    if (!renderer)
    {
        cleanup(window, nullptr);
        return 1;
    }

    // Inicializar PortAudio
    Pa_Initialize();
    AudioData audioData;
    initAudio(audioData, numBars);

    // Abrir y empezar el stream de audio
    PaStream *stream;
    Pa_OpenDefaultStream(&stream, 1, 0, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, audioCallback, &audioData);
    Pa_StartStream(stream);

    bool quit = false;
    SDL_Event e;
    auto lastTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;

    while (!quit)
    {
        // Manejar eventos SDL
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        // Verificar si hay datos de audio listos para ser procesados
        {
            std::unique_lock<std::mutex> lock(audioData.mtx);
            if (audioData.dataReady)
            {
                // Procesar los datos de audio actuales
                if (audioData.samples.size() >= FRAMES_PER_BUFFER)
                {
                    // Copiar los datos a un vector para aplicar el filtro de paso alto
                    std::vector<float> highPassSamples = audioData.samples;

                    float samplingRate = 44100.0f;
                    float scaleFactor = 5.0f;

                    // Realizar la FFT en las muestras de paso alto
                    std::vector<std::complex<float>> highPassFFT = manualFFT(highPassSamples, samplingRate, audioData.numBins, scaleFactor);

                    // Almacenar el resultado de la FFT en fft_output
                    audioData.fft_output = highPassFFT;

                    // Amplificar las frecuencias
                    amplifyFrequencies(audioData.fft_output, LOW_FREQ_AMPLIFICATION, MID_FREQ_AMPLIFICATION, HIGH_FREQ_AMPLIFICATION);

                    // Limpiar el buffer de muestras
                    audioData.samples.clear();
                    audioData.dataReady = false;
                }
            }
        }

        // Limpiar la pantalla
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Renderizar las barras del espectro
        renderBars(renderer, audioData.fft_output, numBars, audioData.smoothedMagnitudes);

        // Actualizar la pantalla
        SDL_RenderPresent(renderer);

        frameCount++;
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = currentTime - lastTime;
        if (elapsed.count() >= 1.0f)
        {
            float fps = frameCount / elapsed.count();
            std::string title = "Spectrum Analyzer - FPS: " + std::to_string(fps);
            SDL_SetWindowTitle(window, title.c_str());
            frameCount = 0;
            lastTime = currentTime;
        }
    }

    // Detener y cerrar el stream de audio
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    // Limpiar recursos de audio y SDL
    cleanupAudio(audioData);
    cleanup(window, renderer);

    return 0;
}