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

int main(void) {
	char *addr;
	int fd1, fd2, i, n, len = 0;
	pid_t *pid;

	fd1 = open("data", O_RDWR | O_CREAT, 0600); 
	fd2 = open("temp", O_RDWR | O_CREAT, 0600);

	pid = mmap(NULL, sizeof(pid_t), PROT_READ, MAP_SHARED, fd1, 0);
	addr = mmap(NULL, 512, PROT_WRITE, MAP_SHARED, fd2, 0);
	ftruncate(fd1, sizeof(pid_t));
	ftruncate(fd2, 512);

	while (*pid == 0);
	printf("writer id = %ld\n", *pid);

	for (i = 0; i < 3; i++) {
		n = read(0, addr + len, 512 - len);
		len += n; 
		if (len >= 512)
			break;
		kill(*pid, SIGUSR1);
	}

	exit(0);
}
