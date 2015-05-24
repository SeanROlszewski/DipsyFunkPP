#include <iostream>
#include <functional>
#include <vector>


template <typename T> struct CallbackState {

    T * buffer;
    T sampleRate;
    uint bufferSize;

    CallbackState(T sr, uint bfSize) : sampleRate(sr), bufferSize(bfSize) {

        buffer = new T [bufferSize];
        memset(buffer, bufferSize, 0);

    }

    ~CallbackState() {

        delete buffer;

    }


};

template <typename T> class DSPModuleController {

private:
    std::vector< std::function<void (CallbackState<T>*)> > callbacks;
    CallbackState<T> * callbackState;

public:

    DSPModuleController (T sampleRate, T bufferSize) {

        callbackState = new CallbackState<T>(sampleRate, bufferSize);
        callbacks.resize(0);

    }

    ~DSPModuleController () {

        std::vector< std::function<void (CallbackState<T>*)> >().swap(callbacks);

    }


    void addDSPCallback(std::function<void (CallbackState<T>*)>  module) {
        callbacks.push_back(module);
    }


    T * renderCallbackChain() {

        if (callbacks.size() > 0) {

            std::for_each(callbacks.begin(), callbacks.end(), [&] (std::function<void (CallbackState<T> *)> callback) {

                callback(callbackState);
                // std::cout << "i\n";

            });

            // return callbackState->buffer;

            return nullptr;

        } else {

            return nullptr;

        }

    }

};
// typedef CallbackState<T> DPSCallbackStateTemp;
typedef float * DSPCallbackReturnTypeFloat;
typedef CallbackState<float> DSPCallbackStateFloat;
typedef std::function<float (CallbackState<float> *)> DSPCallbackFloat;


int main(int argc, char const *argv[]) {


    // Obligatory parameters when working with DSP. :)
    const uint BUFFER_SIZE = 2048;
    const float SAMPLE_RATE = 44100.0;


    // The DSPCallbackController is given a list of callbacks to execute in the order received, and will report its state as it executes.
    DSPModuleController<float> moduleController = DSPModuleController<float>(SAMPLE_RATE, BUFFER_SIZE);

    std::function<void (CallbackState<float> *)> printSample = [] (DSPCallbackStateFloat *callbackState) -> void {

        for (int i = 0; i < callbackState->bufferSize; ++i)
        {

            // Get the sample from the input buffer.
            float sample = callbackState->buffer[i];

            // Do something to the sample.
            sample += 1;
            std::cout << sample << "\n" ;

            // Re-write the sample to the buffer.
            callbackState->buffer[i] = sample;

        }

    };

    moduleController.addDSPCallback(printSample);
    moduleController.renderCallbackChain();

    // Look at output.

    // float * buffer = moduleController.renderCallbackChain();

    // std::for_each(&buffer[0], &buffer[BUFFER_SIZE], [] (float x) {
    //     std::cout << x << "\n";
    // });


    return 0;
}
