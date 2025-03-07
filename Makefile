CC = clang++
FLAGS = -std=c++14 -g -O0 -Wall -Wpedantic -Werror
LIBS = -l portaudio
# SRC =
NAME = HelloDipsyFunk

$(NAME) : dsp.cpp DipsyFunk.hpp PortAudio.hpp DSPModules.hpp StringUtils.hpp
	$(CC) $(FLAGS) -o $(NAME) dsp.cpp $(LIBS)

clean:
	rm $(NAME)
	rm -rf *.dSYM
