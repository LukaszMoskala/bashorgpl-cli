CC=gcc
CXX=g++
#append these flags to command line flags
override CXXFLAGS+=-O3 --std=c++17 -fconcepts -lncurses
PREFIX=/usr/local

all: bashorgpl

bashorgpl.o: main.cpp
	$(CXX) $(CXXFLAGS) -c -o bashorgpl.o main.cpp
bashorgpl: bashorgpl.o
	$(CXX) $(CXXFLAGS) -o bashorgpl bashorgpl.o

install:
	install -m 775 bashorgpl $(PREFIX)/bin/

clean:
	rm -f bashorgpl.o
