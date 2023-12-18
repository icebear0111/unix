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
	struct dirent *d;
	struct stat buf;
	DIR *dp;
	int n;
	
	stat(argv[1], &buf);
        if (S_ISREG(buf.st_mode)) {
		n = remove(argv[1]);
	}	
	else {
		dp = opendir(argv[1]);
		d = readdir(dp);
		chdir(argv[1]);
		while (d != NULL) {
			if(strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0) {
                        	d=readdir(dp);
                        	continue;
               		}
			printf("%s\n", d->d_name);
			n = remove(d->d_name);
			printf("%d\n", n);
			d = readdir(dp);	
		}
		chdir("..");
		rmdir(argv[1]);
	}
	return 0;
}
