all: main

remake: clean main


unitTestDebug: clean main unitTestDebug.o
	g++ -g -std=c++11 unitTestDebug.o debug.o parse.o memory.o trace.o dispatch.o single_op.o double_op.o branch.o jump_sub.o misc_ops.o condition.o -o RunUnitTestDebug.exe

unitTestDebug.o: unitTestDebug.cpp
	g++ -g -std=c++11 -c unitTestDebug.cpp

unitTest: unitTest.o
	g++ -g -std=c++11 unitTest.o parse.o memory.o trace.o debug.o dispatch.o single_op.o double_op.o branch.o jump_sub.o misc_ops.o condition.o -o RunUnitTest.exe

unitTest.o: unitTest.cpp
	g++ -g -std=c++11 -c unitTest.cpp parse.cpp memory.cpp trace.cpp debug.cpp dispatch.cpp single_op.cpp double_op.cpp branch.cpp jump_sub.cpp misc_ops.cpp condition.cpp

opTest: opTest.o
	g++ -g -std=c++11 opTest.o parse.o memory.o trace.o debug.o dispatch.o single_op.o double_op.o branch.o jump_sub.o misc_ops.o condition.o -o RunOpTest.exe

opTest.o: opTest.cpp
	g++ -g -std=c++11 -c opTest.cpp parse.cpp memory.cpp trace.cpp debug.cpp dispatch.cpp single_op.cpp double_op.cpp branch.cpp jump_sub.cpp misc_ops.cpp condition.cpp

dTest: clean main dispatchTest.o
	g++ -g -std=c++11 dispatchTest.o parse.o memory.o trace.o debug.o dispatch.o single_op.o double_op.o branch.o jump_sub.o misc_ops.o condition.o -o dTest.exe

dispatchTest.o: dispatchTest.cpp
	g++ -g -std=c++11 -c dispatchTest.cpp

main: main.o
	g++ -g -std=c++11 main.o parse.o memory.o trace.o debug.o dispatch.o single_op.o double_op.o branch.o jump_sub.o misc_ops.o condition.o -o RunMe.exe

main.o: main.cpp
	g++ -g -std=c++11 -c main.cpp parse.cpp trace.cpp memory.cpp debug.cpp dispatch.cpp single_op.cpp double_op.cpp branch.cpp jump_sub.cpp misc_ops.cpp condition.cpp

filenames := main.o parse.o memory.o trace.o branch.o unitTest.o debug.o dispatch.o single_op.o double_op.o branch.o jump_sub.o trap_int.o condition.o misc_ops.o dispatchTest.o\
unitTestDebug.o RunMe.exe dTest.exe RunUnitTest.exe RunUnitTestDebug.exe opTest.o RunOpTest.exe

files := $(strip $(foreach f,$(filenames),$(wildcard $(f))))

clean:
ifneq ($(files),)
	rm -f $(files)
endif
