#ifndef DSPCONTROLLER_H
#define DSPCONTROLLER_H

#include <iostream>
#include <functional>
#include <vector>
#include <random>
#include <map>

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

template <typename T> class DSPModuleController {

private:
    std::vector< std::function<void (CallbackState<T>*, CallbackParameters<T>*)> > callbacks;
    std::vector< CallbackParameters<T> * > callbackParameters;
    CallbackState<T> * callbackState;

public:

    DSPModuleController (T sampleRate, T bufferSize) {

        callbackState = new CallbackState<T>(sampleRate, bufferSize);
        callbacks.resize(0);

    }

    ~DSPModuleController () {

        std::vector< std::function <void (CallbackState<T>*, CallbackParameters<T>*)> >().swap(callbacks);
        std::vector< CallbackParameters <T> * >().swap(callbackParameters);

    }

    void addDSPCallback(std::function<void (CallbackState<T>*, CallbackParameters<T>*)>  callback,
                                                                  CallbackParameters<T>  *callbackParameter) {

        callbackParameters.push_back(callbackParameter);
        callbacks.push_back(callback);

    }

    T * renderCallbackChain() {

        if (callbacks.size() > 0) {

            typename std::vector< std::function<void (CallbackState<T>*, CallbackParameters<T>*)>>::iterator callbacksIt;
            typename std::vector< CallbackParameters<T> * >::iterator callbackParametersIt;;

            for (   callbacksIt = callbacks.begin(), callbackParametersIt = callbackParameters.begin();
                    callbacksIt != callbacks.end() && callbackParametersIt != callbackParameters.end();
                    ++callbacksIt, ++callbackParametersIt ) {

                auto parameters = *callbackParametersIt;
                std::function<void (CallbackState<T>*, CallbackParameters<T>*)> callback = *callbacksIt;
                callback(callbackState, parameters);

            };

            return callbackState->buffer;

        } else {

            return nullptr;

        }

    }

};

// Convenience Typedefs

typedef float * DSPCallbackReturnTypeFloat;
typedef CallbackState<float> DSPCallbackStateFloat;
typedef CallbackParameters<float> DSPCallbackParametersFloat;
typedef std::function<void (DSPCallbackStateFloat *, DSPCallbackParametersFloat *)> DSPCallbackFloat;

#endif
