CXX = g++
CXXFLAGS = -g3 -Wall
MKDIR = mkdir -p

LIBS = -lm

clean:
	rm -fr *.o a.out core programa dist build

directorios:
	$(MKDIR) build dist

main.o: directorios main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o build/main.o

all: clean main.o
	$(CXX) $(CXXFLAGS) -o dist/programa build/main.o $(LIBS)
	rm -fr build

.DEFAULT_GOAL := all