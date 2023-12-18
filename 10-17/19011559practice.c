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

// (a)
int main(int argc, char  **argv) {
        int i;

        for (i = 0; i < 5; i++) {
                printf("%s\n", argv[1]);
                sleep(1);
        }

        return 0;
}

//(b)
int main(void) {
        int i;
        for (i = 0; i < 5; i++) {
                printf("pid = %d, gid = %d, sid = %d\n", getpid(), getpgrp(), getsid(getpid()));
                sleep(1);
        }

        return 0;
}

//(c)
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

//(1)
int main(void) {
        int i, status;
        pid_t pid;

        for (i = 0; i < 3; i++) {
                pid = fork();
                if (pid == 0 && i == 0) {
                        execl("./p1", "p1", "HelloWorld", (char*)0);
                        exit(0);
                }
                else if (pid == 0 && i == 1) {
                        execl("./p2", "p2", (char*)0);
                        exit(0);
                }
                else if (pid == 0 && i == 2) {
                        execl("./p3", "p3", (char*)0);
                        exit(0);
                }
        }

        for (i = 0; i < 3; i++) {
                pid = wait(&status);
                if (WIFEXITED(status)) {
                        printf("%d...%d\n",pid, WEXITSTATUS(status));
                }
        }

        exit(0);
}
