all: macro

macro: main.o list.o file.o tokens.o lexer.o
	gcc -Wall -g -o macro main.o list.o file.o tokens.o lexer.o

main.o: main.c main.h
	gcc -Wall -g -c main.c

file.o: file.c file.h
	gcc -Wall -g -c file.c

lexer.o: lexer.c lexer.h
	gcc -Wall -g -c lexer.c

list.o: list.c list.h
	gcc -Wall -g -c list.c

tokens.o: tokens.c tokens.h
	gcc -Wall -g -c tokens.c

clean: 
	rm -rf *.o macro