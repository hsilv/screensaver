#ifndef FILTERS_H
#define FILTERS_H

#include <vector>
#include <complex>

void amplifyFrequencies(std::vector<std::complex<float>> &samples, float lowAmplification, float midAmplification, float highAmplification);

#endif