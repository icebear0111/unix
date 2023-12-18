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

int main(void) {
	int i;
	for (i = 0; i < 5; i++) {
		printf("pid = %d, gid = %d, sid = %d\n", getpid(), getpgrp(), getsid(getpid()));
		sleep(1);
	}

	return 0;
}
