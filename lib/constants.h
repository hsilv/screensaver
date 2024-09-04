#ifndef CONSTANTS_H
#define CONSTANTS_H

/* CONSTANTES PARA EL BUEN FUNCIONAMIENTO DEL PROGRAMA,
 * NO SON VARIABLES HARDCODED, SON PARÁMETROS AJUSTABLES PARA EL PROCESAMIENTO
 * DEL AUDIO QUE AYUDAN A VER EL AUDIO DE CIERTAS FORMAS
 * CAMBIAR ESTOS VALORES PUEDE AFECTAR EL FUNCIONAMIENTO DEL PROGRAMA
 */

const int SAMPLE_RATE = 44100;
const int FRAMES_PER_BUFFER = 2048;          // Increased buffer size for better frequency resolution
const int NUM_BARS = 126;                    // Default number of bars
const float SMOOTHING_FACTOR = 0.8f;         // Smoothing factor for exponential smoothing
const float MIN_FREQ = 20.0f;                // Minimum frequency (20 Hz)
const float MAX_FREQ = 20000.0f;             // Maximum frequency (20 kHz)
const float LOW_PASS_ALPHA = 0.002f;         // Alpha value for low-pass filter
const float BAND_PASS_ALPHA = 0.6f;          // Alpha value for band-pass filter
const float HIGH_PASS_AMPLIFICATION = 10.0f; // Alpha value for high-pass filter
const float HIGH_PASS_START_POINT = 0.9f;    // Start point for high-pass filter
const float LOW_FREQ_AMPLIFICATION = 1.5f;   // Amplification factor for low frequencies
const float MID_FREQ_AMPLIFICATION = 3.0f;   // Amplification factor for mid frequencies
const float HIGH_FREQ_AMPLIFICATION = 10.0f; // Amplification factor for high frequencies

#endif