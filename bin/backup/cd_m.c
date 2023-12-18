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
        chdir(argv[1]);
	execlp("/home/account/class/tspark/u19011559/A/main", "main", (char*)0);
	exit(0);
}
