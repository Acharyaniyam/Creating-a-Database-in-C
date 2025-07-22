#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/file.h>
#include "cc.h"
int main(int argc, char*argv[]){
	FILE *fp;
	int id;
	CComp comp;

	if (argc != 2){
		fprintf(stderr, "Eusage: %s ID\n", argv[0]);
		exit(1);
	}

	id = atoi(argv[1]);
	fp = fopen("ccdb", "r+");
	if(fp == NULL){
		perror("ccdb");
		exit(2);
	}

	flock(fileno(fp), LOCK_EX);
	fseek(fp, id * sizeof(CComp), SEEK_SET);
	if (fread(&comp, sizeof(CComp), 1, fp) == 1 && comp.id != 0){
		comp.id = 0;
		fseek(fp, id * sizeof(CComp), SEEK_SET);
		fwrite(&comp, sizeof(CComp), 1, fp);
	} else {
		fprintf(stderr, "Item not found.\n");

	}

	flock(fileno(fp), LOCK_UN);
	fclose(fp);
	return 0;

}
