CC=g++

CXXFLAGS=
LDFLAGS=

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: libcalculate_cpp.a

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

libcalculate_cpp.a: calculate.o
	ar -rc $@ $^

clean:
	rm -rf *.o *.a
