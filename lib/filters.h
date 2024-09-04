#ifndef FILTERS_H
#define FILTERS_H

#include <vector>

void applyLowPassFilter(std::vector<float> &samples, float alpha, float &prevSample, float &prevFilteredSample);
void applyBandPassFilter(std::vector<float> &samples, float alpha, float &prevSample, float &prevFilteredSample);
void applyHighPassFilter(std::vector<float> &samples, float alpha, float &prevSample, float &prevFilteredSample);
void amplifyFrequencies(std::vector<float> &samples, float lowAmplification, float midAmplification, float highAmplification);

#endif // FILTERS_H