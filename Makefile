CC = clang++
FLAGS = -std=c++14 -g -O0 -Wall -Wpedantic -Werror
LIBS = -l portaudio
# SRC =

dsp : dsp.cpp DSPController.hpp PortAudio.hpp
	$(CC) $(FLAGS) -o dsp dsp.cpp $(LIBS)
