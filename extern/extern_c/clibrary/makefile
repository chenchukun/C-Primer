CC=gcc

CXXFLAGS=
LDFLAGS=

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: libcalculate.a

SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c, %.o, $(SOURCES))

$(OBJECTS): %.o:%.c
	$(CC) $(CXXFLAGS) -c $< -o $@

libcalculate.a: calculate.o
	ar -rc $@ $^

clean:
	rm -rf *.o *.a
