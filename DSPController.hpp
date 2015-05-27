#ifndef DSPCONTROLLER_H
#define DSPCONTROLLER_H

#include <functional>
#include <vector>

template <typename T> class SignalMatrix {

private:

    T ** signalMatrix;  // A list of signal inputs to the function.
    unsigned int signalCount; // The number of signals in the matrix.

public:

    SignalMatrix(const unsigned int numberOfInputs, const unsigned int bufferSize) {
        signalCount = numberOfInputs;

        *signalMatrix = new T[signalCount];
        for (int i = 0; i < signalCount; ++i)
        {
            signalMatrix[i] = new T[bufferSize];
        }

    }

    ~SignalMatrix() {

        for (int i = 0; i < signalCount; ++i)
        {
            delete signalMatrix[i];
        }

        delete signalMatrix;
    }



};

template <typename T> struct CallbackState {

    T * buffer;
    T sampleRate;
    int bufferSize;
    SignalMatrix<T> * signalMatrix;

    CallbackState(T sr, int bfSize) : sampleRate(sr), bufferSize(bfSize) {

        buffer = new T [bufferSize];
        memset(buffer, bufferSize, 0);
        // signalMatrix = new SignalMatrix<T>(10, bufferSize);

    }

    ~CallbackState() {

        delete buffer;
        // delete signalMatrix;

    }
};

template <typename T> class CallbackController {

private:

    typedef std::function<void (CallbackState<T>*)> Callback;

    std::vector< Callback > callbacks;
    CallbackState<T> * callbackState;

public:

    CallbackController (T sampleRate, T bufferSize) {

        callbackState = new CallbackState<T>(sampleRate, bufferSize);
        callbacks.resize(0);

    }

    ~CallbackController () {

        delete callbackState;
        std::vector< Callback >().swap(callbacks);

    }

    void addCallback (Callback callback) {

        callbacks.push_back(callback);

    }

    T * renderCallbackChain () {     // TODO: Do I want to render all of the samples at once and return a buffer, or call this function every time I need to render a sample? Probably just a buffer at a time.

        if (callbacks.size() > 0) {

            std::for_each(callbacks.begin(), callbacks.end(), [&] (Callback callback) -> void {

                callback(callbackState);

            });

            return callbackState->buffer;

        } else {

            return nullptr; // TODO: Have this return a buffer of 0s instead of null.

        }

    }

};

// Convenience Typedefs
typedef CallbackState<float> CallbackStateFloat;
typedef std::function<void (CallbackStateFloat *)> CallbackFloat;

#endif
