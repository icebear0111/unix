#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int main(void) {
	if (access("data1", F_OK) == 0) {
		if(access("data1", R_OK|W_OK) == 0) {
			printf("File exists, access to file read and write.");
		}
		else {
			printf("File exists, no access to file read and write.");
		}
	}
	else {
		printf("File not exists.");
	}

	return 0;
}
