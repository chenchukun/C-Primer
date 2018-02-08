CC=g++
CXXFLAGS=-std=c++11 -g -Wall
LDFLAGS=

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: newExpression primitive override

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

newExpression: newExpression.o
	$(CC) $^ -o $@ $(LDFLAGS)

primitive: primitive.o
	$(CC) $^ -o $@ $(LDFLAGS)

override: override.o
	$(CC) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf *.o newExpression primitive override
