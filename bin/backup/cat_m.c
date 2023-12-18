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

int main(int args, char **argv) {
        char buf[512];
        int fd, n;
        fd = open(argv[1], O_RDONLY);

        n = read(fd, buf, 512);
        while(0 < n) {
                write(1, buf, n);
                n = read(fd, buf, 512);
        }
	return 0;
}
