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

pid_t pid[2] = {0};
void catchalarm(int signo);

int main(void){
        char in[50], *res[20]={0};
        int i, status;
	static struct sigaction act;
	sigset_t mask;
	
	act.sa_handler = catchalarm;
	sigaction(SIGINT, &act, NULL);
	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	sigprocmask(SIG_SETMASK, &mask, NULL);

	sigaction(SIGALRM, &act, NULL);

	pid[0] = fork();
	if (pid[0] == 0) {
		execlp("backup_m", "backup_m", (char*)0);
		exit(0);
	}	

        while (1){
               	printf("> ");
        	gets(in);
		if (in[0]=='\0')
			continue;

		i=0;
        	res[i]=strtok(in, " ");
		while (res[i]){
			res[++i]=strtok(NULL, " ");
		}

        	if (strcmp(res[0], "exit")==0){
			if (pid[0] != 0) {
				waitpid(pid[0], &status, 0);
				printf("BACKUP 종료 확인...\n");
			}
        		exit(0);
		}
		else if (strcmp(res[0], "cd_m")==0){
			chdir(res[1]);
		}
		else{
			pid[1] = fork();
			sigprocmask(SIG_UNBLOCK, &mask, NULL);
			if (pid[1] == 0) {
				execvp(res[0], res);
				exit(0);
			}
			else {
				waitpid(pid[1], &status, 0);
				sigprocmask(SIG_SETMASK, &mask, NULL);
			}	
		}
		if (pid[0] != 0) {
			if (waitpid(pid[0], &status, WNOHANG) != 0) {
				pid[0] = 0;
			}
		}
	}
	return 0;
}

void catchalarm(int signo) {
	//alarm(20);
}
