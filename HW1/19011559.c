#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <stdlib.h>

void cat_m(char **res) {
        char buf[512];
        int fd, n;
        fd = open(res[1], O_RDONLY);

        n = read(fd, buf, 512);
        while(0 < n) {
                write(1, buf, n);
                n = read(fd, buf, 512);
        }
}

void cd_m(char **res) {
        chdir(res[1]);
}

void cp_m(char **res) {
        char buf[512];
        int fd1, fd2, n;
	struct stat buf1;
	stat(res[1], &buf1);
	
        fd1 = open(res[1], O_RDONLY);
        if (fd1 == -1)
                return;

        fd2 = open(res[2], O_WRONLY | O_CREAT | O_TRUNC, buf1.st_mode&0777);
        n = read(fd1, buf, 512);
        while(0 < n) {
                write(fd2, buf, n);
                n = read(fd1, buf, 512);
        }
}

void mkdir_m(char **res) {
        mkdir(res[1], 0700);
}

void ls_m(char **res) {
        DIR *dp;
        struct dirent *d;
        struct stat buf;

        dp=opendir(".");
        d=readdir(dp);
        while(d!=NULL){
		if(strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0) {
			d=readdir(dp);
			continue;
		}
                stat(d->d_name, &buf);
                if (S_ISREG(buf.st_mode))
                        printf("F ");
                else
                        printf("D ");

                printf("0%o ", buf.st_mode&0777);
		printf("%d ", buf.st_nlink);
		printf("%d ", buf.st_uid);
		printf("%d ", buf.st_gid);
		printf("%ld ", buf.st_size);
		char *time_str = ctime(&buf.st_mtime);
		time_str[strlen(time_str) - 1] = '\0';
		printf("%s ", time_str);
		printf("%s\n", d->d_name);
                d=readdir(dp);
        }
}

void vi_m(char **res) {
	char buf[512];
	int fd, n;

	fd = open(res[1], O_RDWR | O_CREAT, 0600);

	n = read(fd, buf, 512);
        while(0 < n) {
                write(1, buf, n); 
                n = read(fd, buf, 512);
        }
	
	while(1) {		
		n = read(0, buf, 512);
		buf[n] = '\0';
		if (strcmp(buf, "quit\n") == 0) break;
		write(fd, buf, n);		
	}	
}

int main(void) {
        char in[100], *res[20]={0};
        char *inst[6]={"cat", "cd", "cp", "ls", "mkdir", "vi"};
        void (*f[6])(char **)={cat_m, cd_m, cp_m, ls_m, mkdir_m, vi_m};
        int i;

        while(1) {
                getcwd(in, 99);
                printf("%s> ", in);

                gets(in);
                if (in[0]=='\0')
                        continue;

                i=0;
                res[i]=strtok(in, " ");
                while (res[i]) {
                        i++;
                        res[i]=strtok(NULL, " ");
                }

                if (strcmp(res[0], "exit")==0)
                        exit(0);

                for (i=0;i<6;i++) {
                        if(!strcmp(res[0], inst[i])) {
                                f[i](res);
                                break;
                        }
                }
        }

        return 0;
}
