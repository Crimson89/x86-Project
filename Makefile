all: main

remake: clean main

main: main.o
	g++ -g -std=c++11 main.o parse.o parseUnitTest.o -o RunMe.exe

main.o: main.cpp
	g++ -g -std=c++11 -c main.cpp parse.cpp parseUnitTest.cpp

filenames := main.o parse.o parseUnitTest.o RunMe.exe

files := $(strip $(foreach f,$(filenames),$(wildcard $(f))))

clean:
ifneq ($(files),)
	rm -f $(files)
endif
