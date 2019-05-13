CC=gcc
CXX=g++
#append these flags to command line flags
override CXXFLAGS+=-O3 --std=c++17 -fconcepts
LIBS=-lncurses -lcurl -lX11
PREFIX=/usr/local

all: bashorgpl

bashorgpl.o: main.cpp
	$(CXX) $(CXXFLAGS) -c -o bashorgpl.o main.cpp
x11copy.o: x11copy.c
	$(CC) -c -o x11copy.o x11copy.c
bashorgpl: bashorgpl.o x11copy.o
	$(CXX) $(CXXFLAGS) -o bashorgpl x11copy.o bashorgpl.o $(LIBS)

install:
	install -m 775 bashorgpl $(PREFIX)/bin/

clean:
	rm -f bashorgpl.o x11copy.o
