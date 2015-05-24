
#include "DSPController.hpp"




int main(int argc, char const *argv[]) {


    // Obligatory parameters when working with DSP. :)
    const uint BUFFER_SIZE = 2048;
    const float SAMPLE_RATE = 44100.0;


    // The DSPCallbackController is given a list of callbacks to execute in the order received, and will report its state as it executes.
    DSPModuleController<float> moduleController = DSPModuleController<float>(SAMPLE_RATE, BUFFER_SIZE);

    // A random noise example.
    DSPCallbackFloat randomNoiseGenerator = [] (DSPCallbackStateFloat *callbackState, DSPCallbackParametersFloat *callbackParameters) -> void {

        std::default_random_engine randomGenerator;
        std::uniform_real_distribution<float> distribution(0.0, 1.0);

        for (int i = 0; i < callbackState->bufferSize; ++i)
        {

            float sample = distribution(randomGenerator);
            // Write the sample to the buffer.
            callbackState->buffer[i] = sample;

        }


    };

    moduleController.addDSPCallback(randomNoiseGenerator, nullptr);
    // moduleController.addDSPCallback(incrementSample);

    float * buffer = moduleController.renderCallbackChain();
    if (buffer != nullptr) {

        std::for_each(&buffer[0], &buffer[BUFFER_SIZE - 1], [] (float sample) {
            std::cout << "renderCallbackChain Output: " << sample << "\n";
        });

    }


    return 0;
}
