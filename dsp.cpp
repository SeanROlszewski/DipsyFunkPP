#include "DipsyFunk.hpp" // DSP Callback Prototyping Framework
#include "DSPModules.hpp" // My DSP callbacks. These get added to the callbackController below.
#include "PortAudio.hpp" // For playing back the DSP callbacks

int main(int argc, char const *argv[]) {

    // Obligatory parameters when working with DSP. :)
    const uint BUFFER_SIZE = 256;
    const float SAMPLE_RATE = 44100.0;

    CallbackController<float> * callbackController = new CallbackController<float>(SAMPLE_RATE, BUFFER_SIZE);

    callbackController->addCallback(sineGenerator);
    callbackController->addCallback(pinkNoiseGenerator);
    callbackController->addCallback(LFO);

    // callbackController.renderCallbackChain(); // This is called in PortAudio in this example, but can be called anywhere. It returns a pointer to a buffer.

    PortAudio<float> * portAudioInstace = new PortAudio<float>(SAMPLE_RATE, BUFFER_SIZE, callbackController);
    delete portAudioInstace;
    delete callbackController;

    return 0;
}
