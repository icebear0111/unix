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

void do_child(int N) {
	int i;
	
	for (i = 0; i < 5; i++) {
		printf("pid = %d, gid = %d, sid = %d\n", getpid(), getpgrp(), getsid(getpid()));
		sleep(1);
	}

	exit(N);
}

int main(void) {
	int i, status;
	pid_t pid;

	for (i = 0; i < 3; i++) {
		pid = fork();
		if (pid == 0) {
			do_child(i + 1);
		}
	}

	for (i = 0; i < 3; i++) {
		pid = wait(&status);
		if (WIFEXITED(status)) {
			printf("%d......%d\n", pid, WEXITSTATUS(status));
		}
	}

	exit(0);			
}
