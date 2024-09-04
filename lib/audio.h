#ifndef AUDIO_H
#define AUDIO_H

#include <vector>
#include <fftw3.h>
#include "constants.h"

struct AudioData
{
    std::vector<float> samples;
    fftwf_complex *fft_output;
    fftwf_plan fft_plan;
    std::vector<float> smoothedMagnitudes;
    float prevSampleLowPass;
    float prevFilteredSampleLowPass;
    float prevSampleBandPass;
    float prevFilteredSampleBandPass;
    float prevSampleHighPass;
    float prevFilteredSampleHighPass;
};

void initAudio(AudioData &audioData);
void cleanupAudio(AudioData &audioData);
int audioCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                  const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData);

#endif // AUDIO_H