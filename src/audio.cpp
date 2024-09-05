#include <vector>
#include <portaudio.h>
#include <iostream>
#include "audio.h"
#include "filters.h"
#include "render.h"
#include <cmath>
#include <complex>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <omp.h>

// Bandera para indicar si los datos de audio están listos para ser procesados
bool audioDataReady = false;

/**
 * @brief Realiza una Transformada de Fourier Manual (FFT) en los datos de audio.
 *
 * @param samples Vector de muestras de audio.
 * @param samplingRate Tasa de muestreo de las muestras de audio.
 * @param numBins Número de bins de frecuencia para la FFT.
 * @param scaleFactor Factor de escala para ajustar la amplitud de la FFT.
 * @return std::vector<std::complex<float>> Resultado de la FFT como un vector de números complejos.
 */
std::vector<std::complex<float>> manualFFT(const std::vector<float> &samples, float samplingRate, int numBins, float scaleFactor)
{
    // Vector para almacenar el resultado de la FFT
    std::vector<std::complex<float>> fftResult(numBins);
    size_t N = samples.size();

    // Vector para almacenar los centros de frecuencia de cada bin
    std::vector<float> freqCenters(numBins);
    float minFreq = MIN_FREQ;
    float maxFreq = samplingRate / 2.0f;
    float logMinFreq = std::log10(minFreq);
    float logMaxFreq = std::log10(maxFreq);
    float logFreqStep = (logMaxFreq - logMinFreq) / numBins;

// Calcular los centros de frecuencia en una escala logarítmica
#pragma omp parallel for
    for (int bin = 0; bin < numBins; ++bin)
    {
        freqCenters[bin] = std::pow(11.0f, logMinFreq + bin * logFreqStep);
    }

// Realizar la FFT para cada bin de frecuencia
#pragma omp parallel for
    for (int bin = 0; bin < numBins; ++bin)
    {
        float freqCenter = freqCenters[bin];
        std::complex<float> sum(0.0f, 0.0f);

        // Sumar las contribuciones de cada muestra de audio
        for (size_t n = 0; n < N; ++n)
        {
            float angle = 2.0f * M_PI * freqCenter * n / samplingRate;
            sum += samples[n] * std::complex<float>(cos(angle), -sin(angle));
        }
        // Escalar el resultado y almacenarlo en el vector de resultados
        fftResult[bin] = (sum / static_cast<float>(N)) * scaleFactor;
    }

    return fftResult;
}

/**
 * @brief Callback de PortAudio para procesar el audio en tiempo real.
 *
 * Esta función es llamada por PortAudio en un hilo separado para procesar los datos de audio entrantes.
 *
 * @param inputBuffer Buffer de entrada con los datos de audio.
 * @param outputBuffer Buffer de salida (no utilizado en este caso).
 * @param framesPerBuffer Número de frames por buffer.
 * @param timeInfo Información de tiempo del stream.
 * @param statusFlags Flags de estado del stream.
 * @param userData Puntero a los datos de usuario (AudioData).
 * @return int Código de retorno de PortAudio (paContinue para continuar el stream).
 */
int audioCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                  const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
    // Obtener los datos de usuario (AudioData)
    AudioData *data = (AudioData *)userData;
    const float *in = (const float *)inputBuffer;

    // Copiar los datos de entrada en el buffer de muestras
    {
        std::lock_guard<std::mutex> lock(data->mtx);
        for (unsigned long i = 0; i < framesPerBuffer; ++i)
        {
            data->samples.push_back(in[i]);
        }

        // Notificar al hilo principal si hay suficientes datos
        if (data->samples.size() >= FRAMES_PER_BUFFER)
        {
            data->dataReady = true;
            data->cv.notify_one();
        }
    }

    // Continuar el procesamiento de audio
    return paContinue;
}

/**
 * @brief Inicializa los datos de audio para el procesamiento.
 *
 * Esta función configura los datos de audio necesarios para el procesamiento, incluyendo la reserva de memoria
 * para las muestras de audio y la inicialización de los valores previos para los filtros.
 *
 * @param audioData Referencia a la estructura de datos de audio (AudioData).
 * @param numBins Número de bins de frecuencia para la FFT.
 */
void initAudio(AudioData &audioData, int numBins)
{
    // Reservar espacio en el vector de muestras para evitar realocaciones
    audioData.samples.reserve(FRAMES_PER_BUFFER);

    // Inicializar el vector de magnitudes suavizadas con ceros
    audioData.smoothedMagnitudes.resize(numBins, 0.0f);

    // Inicializar el vector de salida de la FFT con el número de bins especificado
    audioData.fft_output.resize(numBins);

    // Inicializar los valores previos para el filtro de paso bajo
    audioData.prevSampleLowPass = 0.0f;
    audioData.prevFilteredSampleLowPass = 0.0f;

    // Inicializar los valores previos para el filtro de paso de banda
    audioData.prevSampleBandPass = 0.0f;
    audioData.prevFilteredSampleBandPass = 0.0f;

    // Inicializar los valores previos para el filtro de paso alto
    audioData.prevSampleHighPass = 0.0f;
    audioData.prevFilteredSampleHighPass = 0.0f;

    // Establecer el número de bins de frecuencia
    audioData.numBins = numBins;
}

void cleanupAudio(AudioData &audioData)
{
    /* ... */
}