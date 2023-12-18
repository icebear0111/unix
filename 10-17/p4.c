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
        int i, status, n;
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
                n = wait(&status);
                if (WIFEXITED(status)) {
                        printf("%d...%d\n", n, WEXITSTATUS(status));
                }
        }

        exit(0);
}

