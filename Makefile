CC = g++
LD = g++
CPPFLAGS = -std=c++11 -g -Wall -pedantic
LDFLAGS =

all: graph

graph: GraphApp.cpp GraphApp.hpp GraphAlgorithms.cpp structs.hpp GraphAlgorithms.hpp
	$(CC) $(CPPFLAGS) `sdl-config --cflags` GraphApp.cpp GraphAlgorithms.cpp `sdl-config --libs` -lSDL_gfx -o GraphApp

clean :
	rm -rf GraphApp *.o *.dSYM out
