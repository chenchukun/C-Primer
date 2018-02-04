CC=g++
CXXFLAGS=-std=c++11 -Wall -O3
LDFLAGS=

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: volatile

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

volatile: volatile.o
	$(CC) $^ -o $@ $(LDFLAGS)
clean:
	rm -rf *.o volatile
