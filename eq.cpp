#include <iostream>
#include <vector>
#include <cmath>
#include <portaudio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <fftw3.h>

const int SAMPLE_RATE = 44100;
const int FRAMES_PER_BUFFER = 2048;         // Increased buffer size for better frequency resolution
const int NUM_BARS = 126;                   // Default number of bars
const float SMOOTHING_FACTOR = 0.8f;        // Smoothing factor for exponential smoothing
const float MIN_FREQ = 20.0f;               // Minimum frequency (20 Hz)
const float MAX_FREQ = 20000.0f;            // Maximum frequency (20 kHz)
const float LOW_PASS_ALPHA = 0.1f;          // Alpha value for low-pass filter
const float BAND_PASS_ALPHA = 0.5f;         // Alpha value for band-pass filter
const float HIGH_PASS_ALPHA = 0.8f;         // Alpha value for high-pass filter
const float LOW_FREQ_AMPLIFICATION = 1.5f;  // Amplification factor for low frequencies
const float MID_FREQ_AMPLIFICATION = 2.0f;  // Amplification factor for mid frequencies
const float HIGH_FREQ_AMPLIFICATION = 3.5f; // Amplification factor for high frequencies

struct AudioData
{
    std::vector<float> samples;
    fftwf_complex *fft_output;
    fftwf_plan fft_plan;
    std::vector<float> smoothedMagnitudes; // Store smoothed magnitudes for each bar
    float prevSampleLowPass;               // Previous sample for low-pass filter
    float prevFilteredSampleLowPass;       // Previous filtered sample for low-pass filter
    float prevSampleBandPass;              // Previous sample for band-pass filter
    float prevFilteredSampleBandPass;      // Previous filtered sample for band-pass filter
    float prevSampleHighPass;              // Previous sample for high-pass filter
    float prevFilteredSampleHighPass;      // Previous filtered sample for high-pass filter
};

void applyLowPassFilter(std::vector<float> &samples, float alpha, float &prevSample, float &prevFilteredSample)
{
    for (size_t i = 0; i < samples.size(); ++i)
    {
        float currentSample = samples[i];
        float filteredSample = alpha * currentSample + (1.0f - alpha) * prevFilteredSample;
        samples[i] = filteredSample;
        prevSample = currentSample;
        prevFilteredSample = filteredSample;
    }
}

void applyBandPassFilter(std::vector<float> &samples, float alpha, float &prevSample, float &prevFilteredSample)
{
    for (size_t i = 0; i < samples.size(); ++i)
    {
        float currentSample = samples[i];
        float filteredSample = alpha * (prevFilteredSample + currentSample - prevSample);
        samples[i] = filteredSample;
        prevSample = currentSample;
        prevFilteredSample = filteredSample;
    }
}

void applyHighPassFilter(std::vector<float> &samples, float alpha, float &prevSample, float &prevFilteredSample)
{
    for (size_t i = 0; i < samples.size(); ++i)
    {
        float currentSample = samples[i];
        float filteredSample = alpha * (prevFilteredSample + currentSample - prevSample);
        samples[i] = filteredSample;
        prevSample = currentSample;
        prevFilteredSample = filteredSample;
    }
}

void amplifyFrequencies(std::vector<float> &samples, float lowAmplification, float midAmplification, float highAmplification)
{
    size_t third = samples.size() / 3;
    for (size_t i = 0; i < samples.size(); ++i)
    {
        if (i < third)
        {
            samples[i] *= lowAmplification;
        }
        else if (i < 2 * third)
        {
            samples[i] *= midAmplification;
        }
        else
        {
            samples[i] *= highAmplification;
        }
    }
}

static int audioCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
    AudioData *data = (AudioData *)userData;
    const float *in = (const float *)inputBuffer;

    for (unsigned long i = 0; i < framesPerBuffer; ++i)
    {
        data->samples.push_back(in[i]);
    }

    if (data->samples.size() >= FRAMES_PER_BUFFER)
    {
        applyLowPassFilter(data->samples, LOW_PASS_ALPHA, data->prevSampleLowPass, data->prevFilteredSampleLowPass);
        applyBandPassFilter(data->samples, BAND_PASS_ALPHA, data->prevSampleBandPass, data->prevFilteredSampleBandPass);
        applyHighPassFilter(data->samples, HIGH_PASS_ALPHA, data->prevSampleHighPass, data->prevFilteredSampleHighPass);
        amplifyFrequencies(data->samples, LOW_FREQ_AMPLIFICATION, MID_FREQ_AMPLIFICATION, HIGH_FREQ_AMPLIFICATION);
        fftwf_execute(data->fft_plan);
        data->samples.clear();
    }

    return paContinue;
}

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

int main(int argc, char *argv[])
{
    int numBars = NUM_BARS;
    if (argc > 1)
    {
        numBars = std::stoi(argv[1]);
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (TTF_Init() == -1)
    {
        std::cerr << "TTF_Init: " << TTF_GetError() << std::endl;
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Spectrum Analyzer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    Pa_Initialize();
    AudioData audioData;
    audioData.samples.reserve(FRAMES_PER_BUFFER);
    audioData.fft_output = (fftwf_complex *)fftwf_malloc(sizeof(fftwf_complex) * FRAMES_PER_BUFFER);
    audioData.fft_plan = fftwf_plan_dft_r2c_1d(FRAMES_PER_BUFFER, audioData.samples.data(), audioData.fft_output, FFTW_ESTIMATE);
    audioData.smoothedMagnitudes.resize(numBars, 0.0f); // Initialize smoothed magnitudes
    audioData.prevSampleLowPass = 0.0f;                 // Initialize previous sample for low-pass filter
    audioData.prevFilteredSampleLowPass = 0.0f;         // Initialize previous filtered sample for low-pass filter
    audioData.prevSampleBandPass = 0.0f;                // Initialize previous sample for band-pass filter
    audioData.prevFilteredSampleBandPass = 0.0f;        // Initialize previous filtered sample for band-pass filter
    audioData.prevSampleHighPass = 0.0f;                // Initialize previous sample for high-pass filter
    audioData.prevFilteredSampleHighPass = 0.0f;        // Initialize previous filtered sample for high-pass filter

    PaStream *stream;
    Pa_OpenDefaultStream(&stream, 1, 0, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, audioCallback, &audioData);
    Pa_StartStream(stream);

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

    fftwf_destroy_plan(audioData.fft_plan);
    fftwf_free(audioData.fft_output);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}