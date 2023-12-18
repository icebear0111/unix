#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int main(void) {
	char ch1[100], ch2[100];

	scanf("%s", ch1);
	scanf("%s", ch2);

	symlink(ch1, ch2);

	return 0;
}
