#include <stdio.h>
#include <stdlib.h>
#include "file.h"

int main(int argc, char * argv[]){
	if(argc > 2 || argc < 2){
		fprintf(stderr, "ERR: you must provde a filename");
		exit(1);
	}

	FILE * fp;
	fp = fopen(argv[1], "r");
	parseFile(fp);
	fclose(fp);

	return 0;
}