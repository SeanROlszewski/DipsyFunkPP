#ifndef DSPCONTROLLER_H
#define DSPCONTROLLER_H
#include <functional>
#include <map>
#include <vector>

template <typename T> struct CallbackState {

    typedef std::map<std::string, T> ParameterMatrix;

    T sampleRate;
    int bufferSize;
    T * buffer;
    T ** signalMatrix;  // A list of signal inputs with a reference to the signal.
    const unsigned int signalCount = 5;
    // ParameterMatrix parameterMatrix; // A list of parameter values. TODO: Implement this!

    CallbackState(const T sr, const int bfSize) : sampleRate(sr), bufferSize(bfSize) {

        buffer = new T [bufferSize];
        memset(buffer, bufferSize, 0);


        signalMatrix = new T*[signalCount];
        for (int i = 0; i < signalCount; ++i)
        {
            signalMatrix[i] = new T[bufferSize];
        }
        // parameterMatrix = new ParameterMatrix;

    }

    ~CallbackState() {

        delete buffer;

        for (int i = 0; i < signalCount; ++i)
        {
            signalMatrix[i] = nullptr;
        }
        delete signalMatrix;
        // delete parameterMatrix;

    }
};


// The CallbackController is given a list of callbacks to execute,
// and executes the callbacks in the order it's given them.
template <typename T> class CallbackController {
protected:

    T * zeroBuffer;

private:

    typedef std::function<void (CallbackState<T>*)> Callback;

    std::vector< Callback > callbacks;
    CallbackState<T> * callbackState;

public:

    CallbackController (const T sampleRate, const unsigned int bufferSize) {

        callbackState = new CallbackState<T>(sampleRate, bufferSize);
        callbacks.resize(0);

        zeroBuffer = new T[bufferSize];
        memset(zeroBuffer, bufferSize, 0);

    }

    ~CallbackController () {

        delete callbackState;
        delete zeroBuffer;
        std::vector< Callback >().swap(callbacks);

    }

    void addCallback (Callback callback) {

        callbacks.push_back(callback);

    }

    T * renderCallbackChain () noexcept {     // TODO: Do I want to render all of the samples at once and return a buffer, or call this function every time I need to render a sample? Probably just a buffer at a time.

        if (callbacks.size() > 0) {

            std::for_each(callbacks.begin(), callbacks.end(), [&] (Callback callback) -> void {

                callback(callbackState);

            });

            return callbackState->buffer;

        } else { // There are no registered callbacks. Return a zero'd out buffer anyway.

            return zeroBuffer;

        }

    }

};

// Convenience Typedefs
typedef CallbackState<float> CallbackStateFloat;
typedef CallbackState<double> CallbackStateDouble;
typedef std::function<void (CallbackStateFloat *)> CallbackFloat;
typedef std::function<void (CallbackStateDouble *)> CallbackDouble;


#endif
