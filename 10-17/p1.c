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
#include <sys/wait.h>
#include <signal.h>

int main(int argc, char  **argv) {
	int i;

	for (i = 0; i < 5; i++) {
		printf("%s\n", argv[1]);
		sleep(1);
	}

	return 0;
}
