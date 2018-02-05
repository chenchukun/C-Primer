CC=g++
CXXFLAGS=-std=c++11 -g -Wall
LDFLAGS=

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: basic

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

basic: basic.o
	$(CC) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf *.o basic
