#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <ftw.h>

int list(const char *name, const struct stat *status, int type) {
	if(type == FTW_D) {
		printf("%s : %d\n", name, status->st_size);
	}
	else if (type == FTW_F) {
		if (S_IXUSR & status->st_mode || S_IXGRP & status->st_mode || S_IXOTH & status->st_mode) {
			printf("%s : %d\n", name, status->st_size);
		}
	}

	return 0;
}


int main(int argc, char **argv) {
	ftw(argv[1], list, 1);
	printf("%ld : %ld : %ld\n", getpid(), getpgrp(), getsid(getpid()));
	
	return 0;
}
