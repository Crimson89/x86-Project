all: main

remake: clean main

unitTest: unitTest.o
	g++ -g -std=c++11 unitTest.o parse.o memory.o -o RunUnitTest.exe

unitTest.o: unitTest.cpp
	g++ -g -std=c++11 -c unitTest.cpp parse.cpp memory.cpp

main: main.o
	g++ -g -std=c++11 main.o parse.o memory.o trace.o debug.o -o RunMe.exe

main.o: main.cpp
	g++ -g -std=c++11 -c main.cpp parse.cpp trace.cpp memory.cpp debug.cpp

filenames := main.o parse.o memory.o trace.o unitTest.o debug.o RunMe.exe RunUnitTest.exe

files := $(strip $(foreach f,$(filenames),$(wildcard $(f))))

clean:
ifneq ($(files),)
	rm -f $(files)
endif
