#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <unistd.h>
#include "cc.h"
#include <sys/stat.h>
#include <fcntl.h>

int
main(int argc, char *argv[]) {
	CComp newcomp;
	FILE *fp;
	struct stat st;
	int fd;

	if(argc != 8 && !(argc == 8 &&strcmp(argv[1], "-a") == 0)) {
		fprintf(stderr,
			"Usage: %s id name year maker cpu memory desc\n",
			argv[0]);
		exit(1);
	}

	if (strcmp(argv[1], "-a") == 0){
		//Open file manually to get fileze for auto-ID
		fd = open("ccdb", O_RDWR | O_CREAT, 0666);
		if (fd < 0){
			perror("open");
			exit(2);
		}
		if (fstat(fd, &st) < 0){
			perror("fstat");
			close(fd);
			exit(2);
		}
		newcomp.id = st.st_size / sizeof(CComp);

		fp = fdopen(fd, "r+");
		if (fp == NULL){
			perror("fdopen");
			close(fd);
			exit(2);
		}
	}
	else{
		//Manual ID Mode
		newcomp.id = atoi(argv[1]);
		
		fp = fopen("ccdb", "r+");
		if(fp == NULL){
			fp = fopen("ccdb", "w");
			if(fp == NULL){
				perror("ccdb");
				exit(2);
			}
		}
	}

	strncpy(newcomp.name, argv[2], 15);
	newcomp.name[15] = 0;

	newcomp.year = atoi(argv[3]);

	strncpy(newcomp.maker, argv[4], 15);
	newcomp.maker[15] = 0;

	strncpy(newcomp.cpu, argv[5], 7);
	newcomp.cpu[7] = 0;

	newcomp.memory = atoi(argv[6]);

	strncpy(newcomp.desc, argv[7], Descsz - 1);
	newcomp.desc[Descsz - 1] = 0;

	flock(fileno(fp), LOCK_EX);
sleep(10);
	fseek(fp, newcomp.id * sizeof(CComp), SEEK_SET);
	fwrite(&newcomp, 256, 1, fp);
	flock(fileno(fp), LOCK_UN);
	fclose(fp);
}

