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

void catchalarm(int);

int main(void) {
	int i, num, sum = 0;
	static struct sigaction act;
	
	act.sa_handler = catchalarm;
	sigaction(SIGALRM, &act, NULL);

	for (i = 0; i < 10; i++) {
		do {
			alarm(10);
		} while (scanf("%d", &num) < 0);
		alarm(0);
		sum += num;
		printf("sum=%d\n", sum);
	}

	exit(0);
}

void catchalarm(int signo) {
	printf("input !!! input !!! input !!!\n");
}
