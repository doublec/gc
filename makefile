UNAME=$(shell uname -s)

ifeq "$(UNAME)" "Darwin"
INCLUDE=-I/opt/local/include
LIB=-L/opt/local/lib
endif

INCLUDE=
LIB=
CFLAGS=-g

all: libgc.a

libgc.a: gc.o
	ar r libgc.a gc.o

gc.o: gc.cpp gc.h
	g++ $(INCLUDE) $(CFLAGS) -c -o gc.o gc.cpp

clean: 
	rm gc.o
	rm libgc.a
