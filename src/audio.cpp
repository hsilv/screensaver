#include <vector>
#include <portaudio.h>
#include <fftw3.h>
#include "audio.h"
#include "filters.h"
#include "render.h"

int audioCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
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

void initAudio(AudioData &audioData)
{
    audioData.samples.reserve(FRAMES_PER_BUFFER);
    audioData.fft_output = (fftwf_complex *)fftwf_malloc(sizeof(fftwf_complex) * FRAMES_PER_BUFFER);
    audioData.fft_plan = fftwf_plan_dft_r2c_1d(FRAMES_PER_BUFFER, audioData.samples.data(), audioData.fft_output, FFTW_ESTIMATE);
    audioData.smoothedMagnitudes.resize(NUM_BARS, 0.0f);
    audioData.prevSampleLowPass = 0.0f;
    audioData.prevFilteredSampleLowPass = 0.0f;
    audioData.prevSampleBandPass = 0.0f;
    audioData.prevFilteredSampleBandPass = 0.0f;
    audioData.prevSampleHighPass = 0.0f;
    audioData.prevFilteredSampleHighPass = 0.0f;
}

void cleanupAudio(AudioData &audioData)
{
    fftwf_destroy_plan(audioData.fft_plan);
    fftwf_free(audioData.fft_output);
}