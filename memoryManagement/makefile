CC=g++
CXXFLAGS=-std=c++11 -g -Wall
LDFLAGS=

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: newExpression primitive override placementNew complexAllocator allocator cookieTest

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

placementNew: placementNew.o
	$(CC) $^ -o $@ $(LDFLAGS)

complexAllocator: complexAllocator.o
	$(CC) $^ -o $@ $(LDFLAGS)

allocator: allocator.o
	$(CC) $^ -o $@ $(LDFLAGS)

cookieTest: cookieTest.o
	$(CC) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf *.o newExpression primitive override placementNew complexAllocator allocator cookieTest
