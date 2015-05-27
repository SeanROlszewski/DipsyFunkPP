
#include "DSPController.hpp"
#include "portaudio.h"

// A random noise example.
DSPCallbackFloat randomNoiseGenerator = [] (DSPCallbackStateFloat *callbackState) -> void {

    std::default_random_engine randomGenerator;
    std::uniform_real_distribution<float> distribution(-1.0, 1.0);

    for (int i = 0; i < callbackState->bufferSize; ++i)
    {

        float sample = distribution(randomGenerator);
        // Write the sample to the buffer.
        callbackState->buffer[i] = sample;

    }
};

int renderAudio( const void *input,
                 void *output,
                 unsigned long frameCount,
                 const PaStreamCallbackTimeInfo* timeInfo,
                 PaStreamCallbackFlags statusFlags,
                 void *userData ){


    float *outputBuffer = (float *)output;
    CallbackController<float> * callbackController = (CallbackController<float> *) userData;
    float *callbackOutput = callbackController->renderCallbackChain();

    for (int i = 0; i < frameCount; ++i)
    {
        outputBuffer[i] = callbackOutput[i];
    }

    return 0;
}

void printPortAudioErrorMessage(PaError portAudioState) {
    std::cout << "PortAudio Error: \n" << Pa_GetErrorText(portAudioState) << "\n";
}

int runPortAudioWithUserData(const float sampleRate, const int bufferSize, void * userData) {

    PaError portAudioState = Pa_Initialize();
    if (portAudioState != paNoError) {
        printPortAudioErrorMessage(portAudioState);
        return 1;
    }

    PaStream *portAudioStream;
    portAudioState = Pa_OpenDefaultStream( &portAudioStream,
                                           0,
                                           1,
                                           paFloat32,
                                           sampleRate,
                                           bufferSize,
                                           renderAudio,
                                           userData );
    if (portAudioState != paNoError) {
        printPortAudioErrorMessage(portAudioState);
        return 1;
    }


    portAudioState = Pa_StartStream(portAudioStream);
    if (portAudioState != paNoError) {
        printPortAudioErrorMessage(portAudioState);
        return 1;
    }

    Pa_Sleep(3000);

    portAudioState = Pa_StopStream(portAudioStream);
    if (portAudioState != paNoError) {
        printPortAudioErrorMessage(portAudioState);
        return 1;
    }

    portAudioState = Pa_CloseStream(portAudioStream);
    if (portAudioState != paNoError) {
        printPortAudioErrorMessage(portAudioState);
        return 1;
    }

    portAudioState = Pa_Terminate();
    if (portAudioState != paNoError) {
        printPortAudioErrorMessage(portAudioState);
        return 1;
    }

    return 0;
}

int main(int argc, char const *argv[]) {

    // Obligatory parameters when working with DSP. :)
    const uint BUFFER_SIZE = 256;
    const float SAMPLE_RATE = 44100.0;

    // The DSPCallbackController is given a list of callbacks to execute in the order received, and will report its state as it executes.
    CallbackController<float> moduleController = CallbackController<float>(SAMPLE_RATE, BUFFER_SIZE);
    DSPCallbackFloat sineGenerator = [] (DSPCallbackStateFloat *callbackState) -> void {

        const float TWOPI = (2 * 3.14159);
        static float frequency = 440.0;
        static float phase = 0.0;
        static float phaseIncrement = (TWOPI *  frequency) / callbackState->sampleRate;

        for (int i = 0; i < callbackState->bufferSize; ++i)
        {

            float sample = sin(phase);
            // Write the sample to the buffer.
            callbackState->buffer[i] = sample * .5;
            phase += phaseIncrement;

        }

    };

    moduleController.addDSPCallback(sineGenerator);

    // dumpBufferOfSize(moduleController.renderCallbackChain(), BUFFER_SIZE);
    runPortAudioWithUserData(SAMPLE_RATE, BUFFER_SIZE, &moduleController);

    return 0;
}
