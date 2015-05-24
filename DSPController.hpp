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

            });

            return callbackState->buffer;

        } else {

            return nullptr;

        }

    }

};
