#include <vector>
#include <complex>
#include "filters.h"

/**
 * @brief Amplifica las frecuencias de las muestras de audio en diferentes bandas.
 *
 * Esta función divide las muestras de audio en tres bandas de frecuencia (bajas, medias y altas)
 * y aplica diferentes factores de amplificación a cada banda.
 *
 * @param samples Vector de muestras de audio en el dominio de la frecuencia.
 * @param lowAmplification Factor de amplificación para las frecuencias bajas.
 * @param midAmplification Factor de amplificación para las frecuencias medias.
 * @param highAmplification Factor de amplificación para las frecuencias altas.
 */
void amplifyFrequencies(std::vector<std::complex<float>> &samples, float lowAmplification, float midAmplification, float highAmplification)
{
    // Dividir las muestras en tres bandas de frecuencia
    size_t third = samples.size() / 3;
    for (size_t i = 0; i < samples.size(); ++i)
    {
        // Obtener la magnitud y la fase de la muestra actual
        float magnitude = std::abs(samples[i]);
        float phase = std::arg(samples[i]);

        // Aplicar el factor de amplificación correspondiente a cada banda de frecuencia
        if (i < third)
        {
            magnitude *= lowAmplification; // Amplificación para frecuencias bajas
        }
        else if (i < 2 * third)
        {
            magnitude *= midAmplification; // Amplificación para frecuencias medias
        }
        else
        {
            magnitude *= highAmplification; // Amplificación para frecuencias altas
        }

        // Reconstruir la muestra con la nueva magnitud y la fase original
        samples[i] = std::polar(magnitude, phase);
    }
}