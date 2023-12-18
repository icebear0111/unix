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

int main(void){
        char in[50], *res[20]={0};
        int i, status;
	pid_t pid[2]={0};

        while (1){
               	printf("%s > ", getcwd(in, 50));

        	gets(in);
		if (in[0]=='\0')
			continue;

		i=0;
        	res[i]=strtok(in, " ");
		while (res[i]){
			res[++i]=strtok(NULL, " ");
		}

        	if (strcmp(res[0], "exit")==0){
			wait(&status);
        		exit(0);
		}
		else if (strcmp(res[0], "cd_m")==0){
			chdir(res[1]);
		}
		else if (strcmp(res[0], "backup_m")==0){
			if (pid[0] != 0)
				printf("backup in progress ... \n");
			else {	
				pid[0] = fork();
				if (pid[0] == 0) {
					execlp("backup_m", "backup_m", (char*)0);
					exit(0);
				}
			}
		}
		else{
			pid[1] = fork();
			if (pid[1] == 0) {
				execvp(res[0], res);
				exit(0);
			}
			else
				waitpid(pid[1], &status, 0);	
		}
		if (waitpid(pid[0], &status, WNOHANG) != 0)
			pid[0] = 0;
	}
	return 0;
}
