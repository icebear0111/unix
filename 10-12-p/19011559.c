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

int main(int argc, char **argv) { //(a)
        int i, j;

        for (i = 0; i < 3; i++) {
                for (j = 1; argv[j] =! NULL; j++) {
                        printf("%s", argv[j]);
                }
                printf("\n");
        }

        exit(0);
}

/* ———————————————*/

int main(void) { //(b)
        execl("./test1", "test1", "abc", "def", "ghi", (char *)0);
        printf("execution(execl) fails...\n");
        exit(1);
}

/* ———————————————*/

int main(void) { //(c)       
	execlp("test2", "test2", "abc", "def", "ghi", (char *)0);
        printf("execution(execlp) fails...\n");
        exit(1);
}

/* ———————————————*/

int main(void) { //(d)
        char *const av[] = {"test1", "abc", "def", "ghi", (char *)0};

        execv("./test1", av);
        printf("execution(execv) fails...\n");
        exit(1);
}

/* ———————————————*/

int main(void) { //(e)
        char *const av[] = {"test2", "abc", "def", "ghi", (char *)0};

        execvp("test2", av);
        printf("execution(execvp) fails...\n");
        exit(1);
}
