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

int main(int args, char **argv) {
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
               // if (S_ISREG(buf.st_mode))
               //         printf("F ");
               // else
               //         printf("D ");

               // printf("0%o ", buf.st_mode&0777);
               // printf("%d ", buf.st_nlink);
               // printf("%d ", buf.st_uid);
               // printf("%d ", buf.st_gid);
               // printf("%ld ", buf.st_size);
               // char *time_str = ctime(&buf.st_mtime);
               // time_str[strlen(time_str) - 1] = '\0';
               // printf("%s ", time_str);
                printf("%s\n", d->d_name);
                d=readdir(dp);
        }
}
