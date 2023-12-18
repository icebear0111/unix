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

void do_child(void) {
	printf("%d : %d : %d\n", getpid(), getpgid(getpid()), getsid(getpid()));
	exit(0);
}
	
int main(int argc, char **argv) {
	int num, i;
	pid_t pid;

	num = atoi(argv[1]);
	
	for (i = 0; i < num; i++) {
		pid = fork();
		if (pid == 0) {
			do_child();
		}
	}

	for (i = 0; i < num; i++) {
		wait(0);
	}

	return 0;
}
