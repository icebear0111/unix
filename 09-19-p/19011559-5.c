#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int main(void) {
	char symname[100], originalname[100];
	struct stat buf;

	scanf("%s", symname);
	readlink(symname, originalname, 99); 

	lstat(symname, &buf);
	printf("%o, %ld, %s\n", buf.st_mode&0777, buf.st_size, symname);
	stat(originalname, &buf);
	printf("%o, %ld, %s\n", buf.st_mode&0777, buf.st_size, originalname);	

	return 0;
}
