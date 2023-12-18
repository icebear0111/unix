#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int main(void) {
	struct stat buf;

	stat("data1", &buf);
	printf("%o, %ld, %ld\n", buf.st_mode&0777, buf.st_nlink,  buf.st_size);

	return 0;
}
