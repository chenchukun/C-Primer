CC=g++
CXXFLAGS=-std=c++11 -g -Wall
LDFLAGS= ../clibrary/libcalculate.a

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: main

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	CC $(CXXFLAGS) -c $< -o $@

main: main.o
	CC $^ -o $@ $(LDFLAGS)
clean:
	rm -rf *.o main
