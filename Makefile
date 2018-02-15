all: main

main: main.o
	g++ -g main.o -o exe

main.o: main.cpp
	g++ -g -c main.cpp

clean:
		rm -f exe main.o
