#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include "cc.h"

int main(int argc, char *argv[]) {
	FILE *fp;
	CComp comp;
	char *key;

	if(argc != 2){
		fprintf(stderr, "Usage: %s search_string\n", argv[0]);
		exit(1);
	}

	key = argv[1];
	fp = fopen("ccdb", "r");
	if(fp == NULL){
		perror("ccdb");
		exit(2);
	}

	flock(fileno(fp), LOCK_SH);

	while(fread(&comp, sizeof(CComp), 1, fp) == 1) {
		if (comp.id != 0 &&
			(strstr(comp.name, key) ||
			strstr(comp.maker, key) ||
			strstr(comp.cpu, key) ||
			strstr(comp.desc, key))) {

			printf("ID: %d\n", comp.id);
			printf("Name: %s\n", comp.name);
			printf("Year: %d\n", comp.year);
			printf("Maker: %s\n", comp.maker);
			printf("CPU: %s\n", comp.cpu);
			printf("Memory: %d\n", comp.memory);
			printf("Description: %s\n", comp.desc);
			putchar('\n');
		}
	}
	flock(fileno(fp), LOCK_UN);
	fclose(fp);
	return 0;
}

