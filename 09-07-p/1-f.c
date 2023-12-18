#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(void){
	int data[10] = {0}, fd, i;
	
	fd = open("test2", O_RDONLY);
	read(fd, data, 20);
	for (i = 0; i < 10; i++) {
		printf("%d ", data[i]);
	}

	return 0;
}
