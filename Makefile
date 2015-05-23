CC = clang++
FLAGS = -std=c++14 -g -O0 -Wall -Wpedantic -Werror

dsp : dsp.cpp
	$(CC) $(FLAGS) -o dsp dsp.cpp
