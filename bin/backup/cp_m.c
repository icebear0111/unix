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

int main(int argc, char **argv) {
        char buf[512];
        int fd1, fd2, n;
        struct stat buf1;
        stat(argv[1], &buf1);

        fd1 = open(argv[1], O_RDONLY);
        if (fd1 == -1)
                return;

        fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, buf1.st_mode&0777);
        n = read(fd1, buf, 512);
        while(0 < n) {
                write(fd2, buf, n);
                n = read(fd1, buf, 512);
        }

	return 0;
}
