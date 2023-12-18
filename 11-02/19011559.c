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

void catchusr(int);

void do_child(void) {
	int i, pid, gid;
	static struct sigaction act;

	act.sa_handler = catchusr;
	sigaction(SIGUSR1, &act, NULL);
	
	gid = setpgid(0, 0);
	pid = fork();
	pause();		
	
	gid = getpgid(0);
	for (i = 0; i < 3; i++) {
		printf("group id = %d .... process id = %d\n", gid, getpid());
		sleep(1);
	}

	if (pid > 0) {
		pid = wait(0);
		printf("level 2 : process %d safely terminated...\n", pid);
	}
	exit(0);
}
int main(void) {
	int i;
	pid_t pid[2];
	
	for (i = 0; i < 2; i++) {
		pid[i] = fork();
		if (pid[i] == 0) {
			do_child();
		}
	}

	sleep(2);
	kill(-pid[0], SIGUSR1);	
	sleep(5);
	kill(-pid[1], SIGUSR1);


	for (i = 0; i < 2; i++) {
		pid[0] = wait(0);
		printf("level 1 : process %d safely terminated...\n", pid[0]); 
	}
	exit(0);
}

void catchusr(int signo) {
}
