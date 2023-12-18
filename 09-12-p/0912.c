#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(void) {
	char ch = 'X';
	int i, fd;

	fd = open("data1", O_RDWR|O_CREAT, 0600);
	
	for (i = 0; i < 10; i++) {
		write(fd, &ch, 1);
	}

	lseek(fd, 0, SEEK_SET);
	write(fd, "Y", 1);

	lseek(fd, 1, SEEK_END);
	write(fd, "Y", 1);

	for (i = 0; i < 4; i++) {
		lseek(fd, i * 2 + 1, SEEK_SET);
		write(fd, "Z", 1);
	}

	lseek(fd, 15, SEEK_SET);
	write(fd, "T", 1);

	lseek(fd, -2, SEEK_END);
	write(fd, "S", 1);

	lseek(fd, -2, SEEK_CUR);
	write(fd, "W", 1);

	return 0;
}
