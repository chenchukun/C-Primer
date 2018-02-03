CC=g++
CXXFLAGS=-std=c++11 -g -Wall
LDFLAGS=

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: static const

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

static: static.o
	$(CC) $^ -o $@ $(LDFLAGS)

const: const.o
	$(CC) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf *.o static const
