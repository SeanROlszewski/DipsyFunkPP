#include <iostream>
#include <functional>
#include <vector>

// Assumptions made: The DSP callback that gets registered with a DSPModule can only have one input of type T.


template <typename T> class DSPModule {
private:
    std::function<T (T)> callback;

    uint bufferSize;
    T sampleRate;
    T * inputBuffer;
    T * outputBuffer;
    DSPModule * destination;

public:

    DSPModule (uint size, T sr, std::function<T (T)> cb = nullptr) {
        sampleRate = sr;

        bufferSize = size;
        inputBuffer = new T [bufferSize];
        outputBuffer = new T [bufferSize];

        callback = cb;

    }

    ~DSPModule () {

        sampleRate = 0;
        bufferSize = 0;

        delete [] inputBuffer;
        delete [] outputBuffer;
        callback = nullptr;

    }

    void setInputBuffer(T * input) {
        inputBuffer = input;
    }

    void setCallback(std::function<T (T)> dspCallback) {
        callback = dspCallback;
    }

    void clearCallback() {
        callback = nullptr;
    }

    T * render() {

        if(callback != nullptr) {

            for (int i = 0; i < bufferSize; ++i) {
                outputBuffer[i] = callback(inputBuffer[i]);
            }

            return outputBuffer;

        } else {

            throw;

        }
    }
};

template <typename T> class DSPModuleController {

private:
    std::vector< DSPModule<T> * > dspModules;
    T * workingBuffer;
    uint bufferSize;
    T sampleRate;

public:

    DSPModuleController (T size, T sr) {
        bufferSize = size;
        sampleRate = sr;
        workingBuffer = new T [bufferSize];
    }

    ~DSPModuleController () {
        std::vector< DSPModule<T> *>().swap(dspModules);
        delete workingBuffer;
    }

    void addModule(DSPModule<T> * module) {
        dspModules.push_back(module);
    }

    void addModules(DSPModule<T> ** modules, uint numberOfModules) {
        for (int i = 0; i < numberOfModules; ++i) {
            dspModules.push_back(modules[i]);
        }
    }

    T * renderCallbackChain() {

        std::for_each(dspModules.begin(), dspModules.end(), [&] (DSPModule<T> * module) {

            module->setInputBuffer(workingBuffer);
            workingBuffer = module->render();

        });

        return workingBuffer;
    }

};


int main(int argc, char const *argv[]) {

    const uint BUFFER_SIZE = 2048;
    const float SAMPLE_RATE = 44100.0;

    std::function<float (float)> generateImpulse = [] (float x) {
        return 1.0;
    };

    std::function<float (float)> increment = [] (float x) {
        return x + 1.0;
    };


    DSPModule<float> * firstModule = new DSPModule<float>(BUFFER_SIZE, SAMPLE_RATE, generateImpulse);
    DSPModule<float> * secondModule = new DSPModule<float>(BUFFER_SIZE, SAMPLE_RATE, increment);
    DSPModule<float> * thirdModule = new DSPModule<float>(BUFFER_SIZE, SAMPLE_RATE, increment);
    DSPModule<float> * fourthModule = new DSPModule<float>(BUFFER_SIZE, SAMPLE_RATE, increment);


    DSPModuleController<float> moduleController = DSPModuleController<float>(BUFFER_SIZE, SAMPLE_RATE);

    moduleController.addModule(firstModule);
    moduleController.addModule(secondModule);

    DSPModule<float> * modules [] = {thirdModule, fourthModule};
    moduleController.addModules(modules, 2);

    float * buffer = moduleController.renderCallbackChain();

    std::for_each(&buffer[0], &buffer[BUFFER_SIZE], [] (float x) {
        std::cout << x << "\n";
    });


    return 0;
}
