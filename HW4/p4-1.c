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
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>

struct q_entry{
        long mtype;
        int s_id;
        char msg[512];
};

struct q_entry nmessage(int mtype, int s_id, char *str);

void do_sender(int qid, int id){
	char temp[512];
        int i, index=1;
        struct q_entry msg1;

        while(1){
                scanf("%s", temp);

		msg1=nmessage(index, id, temp);
		// (a) message 보내기
		msgsnd(qid, &msg1, 512 + sizeof(int), 0);
		msgsnd(qid, &msg1, 512 + sizeof(int), 0);
                if (strcmp(temp, "talk_quit")==0)
                        break;
		index++;
        }

        exit(0);
}

void do_receiver(int qid, int id, int index){
        struct q_entry msg1;

        while(1){
                // (b) message 받기
                msgrcv(qid, &msg1, 512 + sizeof(int), -index, 0);
                if (msg1.s_id!=id){
                        printf("[sender=%d & msg#=%d] %s\n", msg1.s_id, msg1.mtype, msg1.msg);
                }
		else {
			if (strcmp(msg1.msg, "talk_quit") == 0)
				break;
		}
		// (c) message 받은 후 필요한 작업
		index++;
        }

	exit(0);
}

int main(int argc, char** argv){
        int i, qid, id, index;
	key_t key;
	pid_t pid[2];		
	// 필요한 변수는 추가 선언
       
        key=ftok("key", 5);
        qid=msgget(key, 0600|IPC_CREAT);

	id=atoi(argv[1]);
	index=1;

	// (d) 함수 호출해서 message 주고 받기
	pid[0] = fork();
	if (pid[0] == 0) {
		do_sender(qid, id);
		exit(0);
	}
	pid[1] = fork();
	if (pid[1] == 0) {
		do_receiver(qid, id, index);
		exit(0);
	}
	for (i = 0; i < 2; i++) {
		wait(0);
	}

        exit(0);
}

struct q_entry nmessage(int mtype, int s_id, char *str){
	struct q_entry msg;
	
	msg.mtype=mtype;
	msg.s_id=s_id;
	strcpy(msg.msg, str);

	return msg;
}
