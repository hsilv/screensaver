#ifndef AUDIO_H
#define AUDIO_H

#include <vector>
#include <complex>
#include "constants.h"
#include <condition_variable>
#include <mutex>

/**
 * @brief Estructura para almacenar los datos de audio necesarios para el procesamiento.
 *
 * Esta estructura contiene los buffers de muestras de audio, los resultados de la FFT,
 * las magnitudes suavizadas y los valores previos para los filtros de paso bajo, paso de banda y paso alto.
 */
struct AudioData
{
    std::vector<float> samples;                  ///< Buffer de muestras de audio.
    std::vector<std::complex<float>> fft_output; ///< Resultado de la FFT como un vector de números complejos.
    std::vector<float> smoothedMagnitudes;       ///< Magnitudes suavizadas de las frecuencias.
    float prevSampleLowPass;                     ///< Valor previo de la muestra para el filtro de paso bajo.
    float prevFilteredSampleLowPass;             ///< Valor previo de la muestra filtrada para el filtro de paso bajo.
    float prevSampleBandPass;                    ///< Valor previo de la muestra para el filtro de paso de banda.
    float prevFilteredSampleBandPass;            ///< Valor previo de la muestra filtrada para el filtro de paso de banda.
    float prevSampleHighPass;                    ///< Valor previo de la muestra para el filtro de paso alto.
    float prevFilteredSampleHighPass;            ///< Valor previo de la muestra filtrada para el filtro de paso alto.
    int numBins;                                 ///< Número de bins de frecuencia para la FFT.
    bool dataReady;                              ///< Bandera para indicar si los datos de audio están listos para ser procesados.
    std::mutex mtx;                              ///< Mutex para proteger los datos de audio.
    std::condition_variable cv;                  ///< Variable de condición para notificar al hilo principal.
};

void initAudio(AudioData &audioData, int numBins);
std::vector<std::complex<float>> manualFFT(const std::vector<float> &samples, float samplingRate, int numBins, float scaleFactor);
void cleanupAudio(AudioData &audioData);
int audioCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                  const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData);

#endif // AUDIO_H