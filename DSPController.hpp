#ifndef DSPCONTROLLER_H
#define DSPCONTROLLER_H

#include <iostream>
#include <functional>
#include <vector>
#include <random>
#include <map>

void dumpBufferOfSize( const float * buffer, const int bufferSize ) {

    if (buffer != nullptr) {

        std::for_each(&buffer[0], &buffer[bufferSize - 1], [] (float sample) {
            std::cout << sample << "\n";
        });

    }

}

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

template <typename T> struct CallbackParameters {

    CallbackParameters() {

    }

    ~CallbackParameters (){

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

        std::vector< std::function <void (CallbackState<T>*)> >().swap(callbacks);

    }

    void addDSPCallback(Callback callback) {

        callbacks.push_back(callback);
        std::cout << "Added callback.\n";

    }

    T * renderCallbackChain() {

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

typedef float * DSPCallbackReturnTypeFloat;
typedef CallbackState<float> DSPCallbackStateFloat;
typedef std::function<void (DSPCallbackStateFloat *)> DSPCallbackFloat;

#endif
