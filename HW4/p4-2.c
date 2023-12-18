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
        int cnum;
        int s_id;
        char msg[512];
};

struct q_entry cmessage(int mtype, int cnum);
struct q_entry nmessage(int mtype, int s_id, char *str);

void do_sender(int qid, int id){
	char temp[512];
        int i, cnum, index=1;
        struct q_entry msg1;

	scanf("%s", temp);
	// (e) message 보내기 전 준비
	msgrcv(qid, &msg1, 512 + sizeof(int) * 2, 999999, 0);
        cnum = msg1.cnum;
        msgsnd(qid, &msg1, 512 + sizeof(int) * 2, 0);

        while(1){
                scanf("%s", temp);

		msg1=nmessage(index, id, temp);
               	// (a) message 보내기
               	for (i = 0; i < cnum; i++) {
			msgsnd(qid, &msg1, 512 + sizeof(int), 0);
		}
                if (strcmp(temp, "talk_quit")==0)
                        break;
		index++;
        }

        exit(0);
}

void do_receiver(int qid, int id, int index){
        struct q_entry msg1;

        while(1){
                msgrcv(qid, &msg1, 512 + sizeof(int) * 2, -index, 0);
                if (msg1.s_id!=id){
                        printf("[sender=%d & msg#=%d] %s\n", msg1.s_id, msg1.mtype, msg1.msg);
                }
		else {
			if (strcmp(msg1.msg, "talk_quit") == 0)
				break;
		}
                index++;
        }

	exit(0);
}

int main(int argc, char** argv){
        int i, qid, id, index;
        key_t key;
	// 필요한 변수 추가
	pid_t pid[2];
        struct q_entry msg1, msg2;

        key=ftok("key", 5);
        // (f) message queue 만들고 초기화 작업
	qid = msgget(key, 0600|IPC_CREAT|IPC_EXCL);
        if (qid == -1)
                qid = msgget(key, 0600);
        else {
                msg1 = cmessage(999999, 0);
                msgsnd(qid, &msg1, 512 + sizeof(int) * 2, 0);
        }

	id=atoi(argv[1]);
	index=1;

        // (g) message 통신 전 필요한 작업
	msgrcv(qid, &msg1, 512 + sizeof(int) * 2, 999999, 0);
        msg1.cnum += 1;
        msgsnd(qid, &msg1, 512 + sizeof(int) * 2, 0);

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

        // (h) message 통신 완료 후 message queue 지우기
	msgrcv(qid, &msg1, 512 + sizeof(int) * 2, 999999, 0);
        msg1.cnum -= 1;
	if (msg1.cnum == 0)
		msgctl(qid, IPC_RMID, NULL); 
        else
		msgsnd(qid, &msg1, 512 + sizeof(int) * 2, 0);

        exit(0);
}

struct q_entry cmessage(int mtype, int cnum){
	struct q_entry msg;
	
	msg.mtype=mtype;
	msg.cnum=cnum;
	msg.s_id=0;
	strcpy(msg.msg, "");

	return msg;
}

struct q_entry nmessage(int mtype, int s_id, char *str){
	struct q_entry msg;
	
	msg.mtype=mtype;
	msg.cnum=0;
	msg.s_id=s_id;
	strcpy(msg.msg, str);

	return msg;
}
