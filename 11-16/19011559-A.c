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
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(void) {
	char f[2][3] = {"f1", "f2"};
	int i, in, fd[2];

	for (i = 0; i < 2; i++) {
		mkfifo(f[i], 0600);
	}
	fd[0] = open(f[0], O_WRONLY);
	fd[1] = open(f[1], O_RDONLY);

	for (;;) {
		scanf("%d", &in);
		write(fd[0], &in, sizeof(int));
		if (in == -1)
			exit(0);
		read(fd[1], &in, sizeof(int));
		printf("%d\n", in);
	}

	exit(0);
}
