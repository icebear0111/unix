#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <dirent.h>

void cat_m(char **res){
        char buf[512]; // buffer 크기는 512 bytes 입니다. 이 부분은 수정하지 마세요.
        // cat 명령 수행
        int n, fd;
	fd = open(res[1], O_RDONLY);
	while(0 < (n = read(fd, buf, 512))) {
		write(1, buf, n);
	}
}

void cd_m(char **res){
	// cd 명령 수행
	chdir(res[1]);
}

void mkdir_m(char **res){
	// mkdir 명령 수행
	mkdir(res[1], 0700);
}

void ls_m(char **res){
	// ls 명령 수행
	DIR *dp;
	struct dirent *d;
	dp = opendir(".");
	d = readdir(dp);
	while(d != NULL) {
		printf("%s\n", d->d_name);
		d = readdir(dp);
	}
}

int main(void){    // main 함수는 수정하지 말고, 사용하세요.
        char in[100], *res[20]={0};
        char *inst[4]={"cat", "cd", "mkdir", "ls"};
        void (*f[4])(char **)={cat_m, cd_m, mkdir_m, ls_m};
        int i;

        while (1){
		// current working directory 이름 출력
		getcwd(in, 99);
		printf("%s\n", in);

        	gets(in);
		if (in[0]=='\0')
			continue;
		i=0;
          	res[i]=strtok(in, " ");
		while (res[i]){
			i++;
			res[i]=strtok(NULL, " ");
		}

        	if (strcmp(res[0], "exit")==0)
        		exit(0);
        	for (i=0;i<4;i++){
        		if (!strcmp(res[0], inst[i])){
        			f[i](res);
        			break;
			}
        	}
	}
         return 0;
}
