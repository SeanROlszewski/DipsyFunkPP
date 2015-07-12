#ifndef PORT_AUDIO_WRAPPER_H
#define PORT_AUDIO_WRAPPER_H

#include "portaudio.h"
#include "DipsyFunk.hpp"
#include <iostream>
template <typename T>
class PortAudio {
private:

    static int renderAudio( const void *input,
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

    int runPortAudioWithUserData(const float sampleRate, const int bufferSize, void * userData) {

        // Boiler plate PortAudio code.

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

        Pa_Sleep(10000);

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

    void printPortAudioErrorMessage(PaError portAudioState) {
        std::cout << "PortAudio Error: \n" << Pa_GetErrorText(portAudioState) << "\n";
    }

public:

    bool isPlaying;

    PortAudio(T sampleRate, unsigned int bufferSize, void * userData) {
        isPlaying = false;
        runPortAudioWithUserData(sampleRate, bufferSize, userData);
    }

    ~PortAudio() {
        if (isPlaying) {
            std::cerr << "Terminating PortAudio but still playing.\n";
        }
    }



};




#endif
