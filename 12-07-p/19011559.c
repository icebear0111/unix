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

void do_child (int fd) {
	int in;
	pid_t pid = getpid();
	struct flock lock;
	
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = sizeof(int);
	fcntl(fd, F_SETLKW, &lock);

	lseek(fd, 0, SEEK_SET);
	read(fd, &in, sizeof(int));
	printf("%ld reads %d ... \n", pid, in);
	sleep(5);

	in = in + 10;
	lseek(fd, 0, SEEK_SET);
	write(fd, &in, sizeof(int));
	printf("%ld writes %d ... \n", pid, in);

	lock.l_type = F_UNLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = sizeof(int);
	fcntl(fd, F_SETLK, &lock);

	exit(0);
}	
	
int main(void) {
	int i, in, fd;
	pid_t pid;

	fd = open("data1", O_RDWR|O_CREAT, 0600);
	scanf("%d", &in);
	write(fd, &in, sizeof(int));

	for (i = 0; i < 3; i++) {
		pid = fork();
		if (pid == 0)
			do_child(fd);
	}

	for (i = 0; i < 3; i++)
		wait(0);
		
	lseek(fd, 0, SEEK_SET);
	read(fd, &in, sizeof(int));
	printf("%d\n", in);

	exit(0);
}
