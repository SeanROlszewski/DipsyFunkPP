#ifndef DSP_MODULES_H
#define DSP_MODULES_H

#include "DipsyFunk.hpp"
#include <cmath>
#include <random>

enum Waveform {
    Sine = 0,
    NullWave = 1
};

CallbackFloat sineGenerator = [] (CallbackStateFloat *callbackState) -> void {

    const float TWOPI = (2 * 3.14159);
    static float frequency = 440.0;
    static float phase = 0.0;
    static float phaseIncrement = (TWOPI *  frequency) / callbackState->sampleRate;

    for (int i = 0; i < callbackState->bufferSize; ++i)
    {

        float sample = (sin(phase) + callbackState->buffer[i]) / 2;
        phase += phaseIncrement;
        callbackState->buffer[i] = sample;

    }

};

CallbackFloat LFO = [] (CallbackStateFloat *callbackState) -> void {

    const float TWOPI = (2 * 3.14159);
    static float frequency = 0.5;
    static float phase = 0.0;
    static float phaseIncrement = (TWOPI *  frequency) / callbackState->sampleRate;
    static Waveform waveform = Sine;

    for (int i = 0; i < callbackState->bufferSize; ++i)
    {

        float sample = 0.0;

        if (waveform == Sine) {

            sample = sin(phase) * callbackState->buffer[i];

        }

        phase += phaseIncrement;
        callbackState->buffer[i] = sample;

    }

};

CallbackFloat gaussianNoiseGenerator = [] (CallbackStateFloat *callbackState) -> void {

    const int q = 15;
    const float c1 = (1 << q) - 1;
    const float c2 = ((int)(c1 / 3)) + 1;
    const float c3 = 1.f / c1;

    float random = 0.0;
    float noise = 0.0;

    for (int i = 0; i < callbackState->bufferSize; ++i)
    {

        random = ((float)rand() / (float)(RAND_MAX - 1));
        float rc2 = 3 * (random * c2);
        noise = (2.f * rc2 - 3.f * (c2 - 1.f)) * c3;

        callbackState->buffer[i] = noise * 0.5;
    }

};

CallbackFloat pinkNoiseGenerator = [] (CallbackStateFloat *callbackState) -> void {

    const unsigned int NUMBER_OF_STAGES = 3;
    static const float A[] = { 0.02109238, 0.07113478, 0.68873558 };
    static const float P[] = { 0.3190,  0.7756,  0.9613 };
    static float state[ NUMBER_OF_STAGES ];

    static const float RMI2 = 2.0 / float(RAND_MAX); // + 1.0; // change for range [0,1)
    static const float offset = A[0] + A[1] + A[2];

    for (int i = 0; i < callbackState->bufferSize; ++i)
    {
        float temp = float( rand() );
        state[0] = P[0] * (state[0] - temp) + temp;
        temp = float( rand() );
        state[1] = P[1] * (state[1] - temp) + temp;
        temp = float( rand() );
        state[2] = P[2] * (state[2] - temp) + temp;

        float sample = ( A[0]*state[0] + A[1]*state[1] + A[2]*state[2] )*RMI2 - offset;
        callbackState->buffer[i] = (sample + callbackState->buffer[i]) * .5;
    }

};

#endif
