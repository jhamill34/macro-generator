#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "tokens.h"
#include "lexer.h"
#include "file.h"


void parseFile(FILE * fp){
	char * line;
	int linenum = -1;
	int colnum;

	// printf("Line\tCol\tChar\n");
	while(feof(fp) == 0){
		line = readline(fp);
		linenum++;

		// Iterate through each char in the line up until the newline
		for(colnum = 0; line[colnum] != '\n'; colnum++){
			printf("%d\t%d\t%c\n", linenum, colnum, line[colnum]);	
		}

		free(line);
	}

	printf("\n");
	return;
}

int getlinelength(FILE * fp){
	// get the inital starting point
	fpos_t  current;
	fgetpos(fp, &current);
	int length = 0;
	int ch;
	
	// read in characters one at a time
	// untill you see a newline character
	ch = fgetc(fp);
	while(ch != '\n' && ch != EOF){
		length++;
		ch = fgetc(fp);
	}
	length++; // make sure we capture the newline

	// return from where you started
	fsetpos(fp, &current);

	return length;
}

// returns a pointer to a string that points to a 
// pre-allocated amount of data representing the next line in 
// the passed file pointer
char * readline(FILE * fp){
	char * line;
	int length = getlinelength(fp) + 1;
	line = (char *) malloc(sizeof(char) * length);

	if(fgets(line, length + 1, fp)){
		return line;
	}else{
		strcpy(line, "\0");
		return line;
	}
}