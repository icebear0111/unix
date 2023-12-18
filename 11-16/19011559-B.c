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

	fd[0] = open(f[0], O_RDONLY);
	fd[1] = open(f[1], O_WRONLY);

	for(;;) {
		read(fd[0], &in, sizeof(int));
		printf("%d\n", in);
		if (in == -1)
			exit(0);
		in = in + 8;
		write(fd[1], &in, sizeof(int));
	}
	
	exit(0);
}
