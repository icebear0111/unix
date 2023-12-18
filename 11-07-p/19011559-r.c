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
	int fd, i;
	int *addr;

	fd = open("temp", O_RDWR|O_CREAT, 0600);
	addr = mmap(NULL, 40, PROT_WRITE, MAP_SHARED, fd, 0);
	ftruncate(fd, sizeof(int)*10);

	for (i = 0; i < 10; i++)
		scanf("%d", addr + i);

	exit(0);
}
