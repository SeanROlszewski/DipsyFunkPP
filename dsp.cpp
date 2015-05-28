
#include "DSPController.hpp" // DSP Callback Prototyping Framework
#include "DSPModules.hpp" // My DSP callbacks. These get added to the callbackController below.
#include "PortAudio.hpp" // For playing back the DSP callbacks

int main(int argc, char const *argv[]) {

    // Obligatory parameters when working with DSP. :)
    const uint BUFFER_SIZE = 256;
    const float SAMPLE_RATE = 44100.0;

    CallbackController<float> callbackController = CallbackController<float>(SAMPLE_RATE, BUFFER_SIZE);

    callbackController.addCallback(sineGenerator);
    callbackController.addCallback(pinkNoiseGenerator);
    callbackController.addCallback(LFO);

    // callbackController.renderCallbackChain(); // This is called in runPortAudioWithUserData here, but can be called anywhere.

    // The only thing worth checking out in this function is where callbackController gets passed to PortAudio's callback.
    runPortAudioWithUserData(SAMPLE_RATE, BUFFER_SIZE, &callbackController);

    return 0;
}
