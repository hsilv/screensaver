#include <vector>
#include "filters.h"

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