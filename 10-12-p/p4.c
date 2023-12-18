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

int main(void) {
	char *const av[] = {"3", "abc", "def", "ghi", (char *)0};

	execvp("test2", av);
	printf("execution(execvp) fails...\n");
	exit(1);
}
