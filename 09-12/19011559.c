#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int main(void) {
	int i, fd, data1[10], data2[10];

	for (i = 0; i < 10; i++) {
		scanf("%d", &data1[i]);
	}

	fd = open("test", O_RDWR|O_CREAT|O_TRUNC, 0600);

	for (i = 0; i < 5; i++) {
		write(fd, &data1[i], sizeof(int));
		lseek(fd, sizeof(int), SEEK_CUR);
	}

	lseek(fd, -9 * sizeof(int), SEEK_CUR);

	for (i = 5; i < 10; i++) {
		write(fd, &data1[i], sizeof(int));
		lseek(fd, sizeof(int), SEEK_CUR);
	}

	lseek(fd, 0, SEEK_SET);

	read(fd, data2, 10 * sizeof(int));

	for (i = 0; i < 10; i++) {
		printf("%-5d", data1[i]);
	}
	printf("\n");
	
	for (i = 0; i < 10; i++) {
		printf("%-5d", data2[i]);
	}
	printf("\n");
		
	return 0;
}
