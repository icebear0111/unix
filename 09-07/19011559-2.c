#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(void) {
	char n1[50], n2[50], buf[512], in;
	int i, fd1, fd2, n;

	scanf("%s", n1);
	scanf("%s", n2);

	fd1 = open(n1, O_RDONLY);
	if (fd1 == -1) {
		printf("%s : No such file\n", n1);
		return 0;
	}

	fd2 = open(n2, O_WRONLY|O_CREAT|O_EXCL, 0600); 
	if (fd2 == -1) {
		printf("%s : File already exists... Do you want to overwrite?\n", n2);
		getchar();
		scanf("%c", &in);
		if (in != 'Y')
			return 0;
		fd2 = open(n2, O_WRONLY|O_CREAT|O_TRUNC, 0600);
	}

	n = read(fd1, buf, 512);
	while(n > 0) {
		write(fd2, buf, n);
		n = read(fd1, buf, 512);
	}
		
	return 0;
} 

