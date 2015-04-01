all: macro

macro: main.o parse.o tokens.o composite.o execute.o
	g++ -Wall -g -o macro main.o parse.o tokens.o composite.o execute.o

main.o: main.cpp composite.h 
	g++ -Wall -g -c main.cpp

parse.o: parse.cpp parse.h 
	g++ -Wall -g -c parse.cpp

tokens.o: tokens.cpp tokens.h
	g++ -Wall -g -c tokens.cpp	

composite.o: composite.cpp composite.h
	g++ -Wall -g -c composite.cpp

execute.o: execute.cpp execute.h
	g++ -Wall -g -c execute.cpp

clean: 
	rm -rf *.o macro

package: clean
	mkdir macro
	mkdir macro/examples
	cp *.h *.c *.cpp Makefile macro 
	cp examples/* macro/examples
	tar -cjvf macro-archive.tar.bz ./macro
	rm -rf macro 

hidden: package
	mv macro-archive.tar.bz notes.txt 
