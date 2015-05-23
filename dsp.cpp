#include <iostream>
#include <functional>

// Assumptions made: The DSP callback that gets registered with a DSPModule can only have one input of type T.


template <typename T> class DSPModule {
private:
    std::function<T (T)> callback;

    uint bufferSize;
    T sampleRate;
    T * inputBuffer;
    T * outputBuffer;

public:

    DSPModule (uint size, T sr) {
        sampleRate = sr;

        bufferSize = size;
        inputBuffer = new T [bufferSize];
        outputBuffer = new T [bufferSize];

        callback = nullptr;

    }

    ~DSPModule () {

        sampleRate = 0;
        bufferSize = 0;

        delete [] inputBuffer;
        delete [] outputBuffer;
        callback = nullptr;

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

int main(int argc, char const *argv[]) {

    std::function<float (float)> callback = [] (float x) {
        return x * x;
    };

    const uint BUFFER_SIZE = 2048;
    const float SAMPLE_RATE = 44100.0;

    // float * buffer = new float [BUFFER_SIZE];

    DSPModule<float> myModule = DSPModule<float>(BUFFER_SIZE, SAMPLE_RATE);

    myModule.setCallback(callback);
    float * buffer = myModule.render();

    std::for_each(&buffer[0], &buffer[BUFFER_SIZE], [] (float x) {
        std::cout << x << "\n";
    });


    myModule.clearCallback();


    return 0;
}
