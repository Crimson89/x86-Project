all: main

main: main.o
	g++ -g -std=c++11 main.o -o RunMe.exe

main.o: main.cpp
	g++ -g -std=c++11 -c main.cpp

filenames := main.o RunMe.exe

files := $(strip $(foreach f,$(filenames),$(wildcard $(f))))

clean:
ifneq ($(files),)
	rm -f $(files)
endif
