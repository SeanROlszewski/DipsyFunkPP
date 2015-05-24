#include <iostream>
#include <functional>
#include <vector>
#include <random>
#include <map>

#include "DSPController.hpp"

typedef float * DSPCallbackReturnTypeFloat;
typedef CallbackState<float> DSPCallbackStateFloat;
typedef std::function<void (CallbackState<float> *)> DSPCallbackFloat;


int main(int argc, char const *argv[]) {


    // Obligatory parameters when working with DSP. :)
    const uint BUFFER_SIZE = 2048;
    const float SAMPLE_RATE = 44100.0;


    // The DSPCallbackController is given a list of callbacks to execute in the order received, and will report its state as it executes.
    DSPModuleController<float> moduleController = DSPModuleController<float>(SAMPLE_RATE, BUFFER_SIZE);

    DSPCallbackFloat incrementSample = [] (DSPCallbackStateFloat *callbackState) -> void {

        for (int i = 0; i < callbackState->bufferSize; ++i)
        {

            // Get the sample from the input buffer. (If need be)
            float sample = callbackState->buffer[i];

            // Do something to the sample.
            sample += 1;

            // Re-write the sample to the buffer.
            callbackState->buffer[i] = sample;

        }

    };

    DSPCallbackFloat printSample = [] (DSPCallbackStateFloat *callbackState) -> void {

        for (int i = 0; i < callbackState->bufferSize; ++i)
        {

            // Get the sample from the input buffer.
            float sample = callbackState->buffer[i];

            // Do something to the sample.
            std::cout << "sample number " << i << "value : " << sample << "\n";

        }

    };

    DSPCallbackFloat randomNoiseGenerator = [] (DSPCallbackStateFloat *callbackState) -> void {

        std::default_random_engine randomGenerator;
        std::uniform_real_distribution<float> distribution(0.0, 1.0);

        for (int i = 0; i < callbackState->bufferSize; ++i)
        {

            float sample = distribution(randomGenerator);
            // Write the sample to the buffer.
            callbackState->buffer[i] = sample;

        }


    };

    DSPCallbackFloat sineWaveGenerator = [] (DSPCallbackStateFloat *callbackState) -> void {

        // static float phase = 0.0;
        // static float phaseIncrement = TWOPI * (callbackState->sampleRate);

        for (int i = 0; i < callbackState->bufferSize; ++i)
        {

            float sample = 0.0;
            // Write the sample to the buffer.
            callbackState->buffer[i] = sample;


        }

    };

    moduleController.addDSPCallback(randomNoiseGenerator);
    // moduleController.addDSPCallback(incrementSample);

    float * buffer = moduleController.renderCallbackChain();
    if (buffer != nullptr) {

        std::for_each(&buffer[0], &buffer[BUFFER_SIZE - 1], [] (float sample) {
            std::cout << "renderCallbackChain Output: " << sample << "\n";
        });

    }


    return 0;
}
