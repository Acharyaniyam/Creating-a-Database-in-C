#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include "cc.h"

int
main(int argc, char *argv[]) {
	FILE *fp;
	CComp comp;
	int y1, y2;

	if(argc != 3) {
		fprintf(stderr, "Usage: %s start_year end_year\n", argv[0]);
		exit(1);
	}

	y1 = atoi(argv[1]);
	y2 = atoi(argv[2]);

	if(y1 > y2) {
		int temp = y1;
		y1 = y2;
		y2 = temp;
	}

	fp = fopen("ccdb", "r");
	if(fp ==NULL) {
		perror("ccdb");
		exit(2);
	}

	flock(fileno(fp), LOCK_SH);

	while(fread(&comp, sizeof(CComp), 1, fp) == 1) {
		if (comp.id != 0 && comp.year >= y1 && comp.year <= y2) {
			printf("ID: %d\n", comp.id);
			printf("Name: %s\n", comp.name);
			printf("Year: %d\n", comp.year);
			printf("Maker: %s\n", comp.maker);
			printf("CPU: %s\n", comp.cpu);
			printf("Memory: %d\n", comp.memory);
			printf("Description: %s\n", comp.desc);
		}
	}
	flock(fileno(fp), LOCK_UN);
	fclose(fp);
	return 0;
}
