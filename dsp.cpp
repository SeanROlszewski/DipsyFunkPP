
#include "DSPController.hpp"
#include "PortAudio.hpp"
#include <cmath>

enum LFOParameterWaveform {
    LFOParameterWaveformSine = 0,
    LFOParameterWaveformSquare = 1
};

int main(int argc, char const *argv[]) {
    srand ( time(NULL) );
    // Obligatory parameters when working with DSP. :)
    const uint BUFFER_SIZE = 256;
    const float SAMPLE_RATE = 44100.0;

    CallbackFloat sineGenerator = [] (CallbackStateFloat *callbackState) -> void {

        const float TWOPI = (2 * 3.14159);
        static float frequency = 440.0;
        static float phase = 0.0;
        static float phaseIncrement = (TWOPI *  frequency) / callbackState->sampleRate;

        for (int i = 0; i < callbackState->bufferSize; ++i)
        {

            float sample = (sin(phase) + callbackState->buffer[i]) / 2;
            callbackState->buffer[i] = sample;
            phase += phaseIncrement;

        }

    };

    CallbackFloat LFO = [] (CallbackStateFloat *callbackState) -> void {

        const float TWOPI = (2 * 3.14159);
        static float frequency = 0.5;
        static float phase = 0.0;
        static float phaseIncrement = (TWOPI *  frequency) / callbackState->sampleRate;
        static LFOParameterWaveform waveform = LFOParameterWaveformSine;

        for (int i = 0; i < callbackState->bufferSize; ++i)
        {

            float sample = 0.0;

            if (waveform == LFOParameterWaveformSine) {

                sample = sin(phase) * callbackState->buffer[i];

            } else if (waveform == LFOParameterWaveformSquare) {

                sample = 0.0;

                // Idea: use integer overflow to avoid conditional jumps.

                typedef unsigned long ui32;

                float frequency = 440.0f;
                float one = 1.0f;
                ui32 intOver = 0L;
                ui32 intIncr = (ui32)(4294967296.0 / callbackState->sampleRate / frequency);

                // loop:
                (*((ui32 *)&one)) &= 0x7FFFFFFF; // mask out sign bit
                (*((ui32 *)&one)) |= (intOver & 0x80000000);
                intOver += intIncr;

            }

            callbackState->buffer[i] = sample;
            phase += phaseIncrement;

        }

    };

    CallbackFloat gaussianNoiseGenerator = [] (CallbackStateFloat *callbackState) -> void {

        const static int q = 15;
        const static float c1 = (1 << q) - 1;
        const static float c2 = ((int)(c1 / 3)) + 1;
        const static float c3 = 1.f / c1;

        float random = 0.0;
        float noise = 0.0;

        for (int i = 0; i < callbackState->bufferSize; ++i)
        {
            random = ((float)rand() / (float)(RAND_MAX - 1));
            noise = (2.f * ((random * c2) + (random * c2) + (random * c2)) - 3.f * (c2 - 1.f)) * c3;
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
            callbackState->buffer[i] = sample;
        }

    };

    // CallbackFloat sawtoothGenerator = [] (CallbackStateFloat *callbackState) -> void {
    //     double phase=0;
    //     double dphase = 0;
    //     double freq = 440.0;
    //     double compensation = 0.0;
    //     double aw0=0,aw1=0,ax0=0,ax1=0,ay0=0,ay1=0,az0=0,az1=0,sortie;
    //     short sample;
    //     // int sr=44100;       //sample rate (Hz)
    //     // double f_debut=400.0;//start freq (Hz)
    //     // double f_fin=sr/6.0;//end freq (Hz)
    //     // double octaves=log(f_fin/f_debut)/log(2.0);

    //     double phaseIncrement = ((2 * 3.1415) * freq) / callbackState->sampleRate;

    //     const int numberOfSamples = callbackState->bufferSize;
    //     for( int i = 0; i < numberOfSamples; i++ )
    //     {
    //         // //exponential frequency sweep
    //         // //Can be replaced by anything you like.
    //         // freq=f_debut*pow(2.0,octaves*i/(numberOfSamples));
    //         // dphase=freq*(2.0/sr);     //normalised phase increment
    //         // phase+=dphase;            //phase incrementation
    //         // if(phase>1.0) phase-=2.0; //phase wrapping (-1,+1)




    //         //polynomial calculation (extensive continuity at -1 +1)
    //         //        7       1  3    1   5
    //         //P(x) = --- x - -- x  + --- x
    //         //       360     36      120
    //         aw0 =phase*(7.0/360.0 + phase*phase*(-1/36.0 + phase*phase*(1/120.0)));


    //         // quad differentiation (first order high pass filters)
    //         ax0=aw1-aw0; ay0=ax1-ax0; az0=ay1-ay0; sortie=az1-az0;


    //         //compensation of the attenuation of the quad differentiator
    //         //this can be calculated at "control rate" and linearly
    //         //interpolated at sample rate.
    //         compensation = 1.0 / (dphase * dphase * dphase * dphase);
    //         // compensation and output
    //         sample = (short)(15000.0*compensation*sortie);



    //         callbackState->buffer[i] = sample;
    //         phase += phaseIncrement;


    //         //old memories of differentiators
    //         aw1=aw0; ax1=ax0; ay1=ay0; az1=az0;
    //     }
    // };



    // The DSPCallbackController is given a list of callbacks to execute in the order received, and will report its state as it executes.
    CallbackController<float> callbackController = CallbackController<float>(SAMPLE_RATE, BUFFER_SIZE);

    callbackController.addCallback(sineGenerator);
    callbackController.addCallback(LFO);

    runPortAudioWithUserData(SAMPLE_RATE, BUFFER_SIZE, &callbackController);

    return 0;
}
