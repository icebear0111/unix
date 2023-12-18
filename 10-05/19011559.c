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
	int cnt;
	struct dirent *d;
	DIR *dp;

	if (type == FTW_D) {
		cnt = 1; //일단 비어있다고 가정
		dp = opendir(name);
		while(d = readdir(dp)) {
			if(strcmp(d->d_name, ".") != 0 && strcmp(d->d_name, "..") != 0) {
				cnt = 0;
				break;
			}
		}

		closedir(dp);

		if (cnt	== 1) {
			printf("%s\n", name);
		}			
	}

	return 0;
}

int main(void) {
	ftw(".", list, 1);

	return 0;
}
