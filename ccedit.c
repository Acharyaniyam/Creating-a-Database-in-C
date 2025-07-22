#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include "cc.h"

#define INPUT_BUF 256

void prompt_edit(char *field_name, char *value, int maxlen) {
	char buf[INPUT_BUF];
	printf("%s [%s]: ", field_name, value);
	if (fgets(buf, INPUT_BUF, stdin)) {
		buf[strcspn(buf, "\n")] = 0; //strip newline
		if (strlen(buf) > 0){
			strncpy(value, buf, maxlen);
			value[maxlen] = 0;
		}
	}
}

int main(int argc, char *argv[]) {
	FILE *fp;
	CComp comp;
	int id;
	char buf[INPUT_BUF];

	if(argc != 2) {
		fprintf(stderr, "Usage: %s ID\n", argv[0]);
		exit(1);
	}

	id = atoi(argv[1]);
	fp = fopen("ccdb", "r+");
	if(fp == NULL) {
		perror("ccdb");
		exit(2);
	}

	flock(fileno(fp), LOCK_EX);
	fseek(fp, id * sizeof(CComp), SEEK_SET);

	if(fread(&comp, sizeof(CComp), 1, fp) != 1 || comp.id == 0) {
		fprintf(stderr, "Record not found.\n");
		flock(fileno(fp), LOCK_UN);
		fclose(fp);
		exit(3);
	}

	printf("Editing record ID %d\n", comp.id);

	prompt_edit("Name", comp.name, 15);
	printf("Year [%d]: ", comp.year);
	if (fgets(buf, INPUT_BUF, stdin)) {
		int y = atoi(buf);
		if (y > 0) comp.year = y;
	}

	prompt_edit("Maker", comp.maker, 15);
	prompt_edit("CPU", comp.cpu, 7);
	printf("Memory [%d]: ", comp.memory);
	if (fgets(buf, INPUT_BUF, stdin)) {
		int m = atoi(buf);
		if (m > 0) comp.memory = m;
	}
	prompt_edit("Description", comp.desc, Descsz - 1);

	//Overwrite edited record
	fseek(fp, id * sizeof(CComp), SEEK_SET);
	fwrite(&comp, sizeof(CComp), 1, fp);

	flock(fileno(fp), LOCK_UN);
	fclose(fp);
	return 0;
}
