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

void catchsig(int);

void do_child(int i) {
	int j, pid;
	static struct sigaction act;
	
	act.sa_handler = catchsig;
	sigaction(SIGUSR1, &act, NULL);
	
	printf("%d-th child is created...\n", i);
	pause();

	for(j = 0; j < 3; j++) {
		printf("pid: %d\n", getpid());
	}

	exit(i);
}

int main(void) {
	int i, cid, status;
	pid_t pid[3];

	for (i = 0; i < 3; i++) {
		pid[i] = fork();
		if (pid[i] == 0) {
			do_child(i);
		}
	}
	
	for (i = 0; i < 3; i++) {
		sleep(1);
		kill(pid[i], SIGUSR1);
	}
	
	for (i = 0; i < 3; i++) {
		cid = wait(&status);
		printf("child id = %d, exit status = %d\n", cid, WEXITSTATUS(status));
	}

	exit(0);
}

void catchsig(int signo) {
}
