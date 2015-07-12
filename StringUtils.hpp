#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <iostream>

void dumpBufferOfSize( const float * buffer, const int bufferSize ) {

    if (buffer != nullptr) {

        std::for_each(&buffer[0], &buffer[bufferSize - 1], [] (float sample) {
            std::cout << sample << "\n";
        });

    }
}



#endif
