CC=gcc
CXX=g++
#append these flags to command line flags
override CXXFLAGS+=-O2 --std=c++17 -fconcepts
LIBS=-lncurses -lcurl
PREFIX=/usr/local

all: bashorgpl

bashorgpl.o: main.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^
bashorgpl: bashorgpl.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

install: bashorgpl
	install -m 775 $^ $(PREFIX)/bin/

clean:
	rm -f *.o
