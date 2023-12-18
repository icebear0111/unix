#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <ftw.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	int i, j;
	
	for (i = 0; i < 3; i++) {
		for (j = 1; j < argc; j++) {
			printf("%s", argv[j]);
		}
		printf("\n");
	}

	exit(0);
}
