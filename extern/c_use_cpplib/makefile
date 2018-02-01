CC=gcc
CXXFLAGS=-g -Wall
LDFLAGS= ../cpplibrary/libcalculate_cpp.a

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: main

SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c, %.o, $(SOURCES))

$(OBJECTS): %.o:%.c
	$(CC) $(CXXFLAGS) -c $< -o $@

main: main.o
	$(CC) $^ -o $@ $(LDFLAGS)
clean:
	rm -rf *.o main
