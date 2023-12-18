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

int main(void) {
	int fd, i;
	char c;

	fd = open("data", O_RDONLY);
	for (i = 0; i < 10; i++) {
		read(fd, &c, sizeof(char));
		printf("%c\n", c);
		sleep(1);
	}

	return 0;
}
