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
#include <sys/sem.h>
#include <sys/shm.h>

int main(void) {
	int i, in, fd, turn, n;
	pid_t pid;
	struct flock lock;

	fd = open("turn1", O_RDWR|O_CREAT, 0600);
	
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 4;
	lock.l_type = F_WRLCK;
	fcntl(fd, F_SETLKW, &lock);

	lseek(fd, 0, SEEK_SET);
	read(fd, &in, sizeof(int));
	
	pid = getpid();
	for (i = 0; i < 5; i++) {
		sleep(1);
		printf("%ld\n", pid);
	}

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	
	exit(0);
}
