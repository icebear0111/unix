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
#include <limits.h>

struct q_entry{
        long mtype;
        int cnum;
        int s_id;
	int msgnum;
        char msg[512];
};

struct q_entry cmessage(int mtype, int cnum, int msgnum);
struct q_entry nmessage(int mtype, int s_id, char *str, int msgnum);

void do_sender(int qid, int id){
	char temp[512];
        int i, cnum, talker, index=1, msgnum;
        struct q_entry msg1;

	msgrcv(qid, &msg1, 3 * sizeof(int) + 512, INT_MAX, 0);
        msgnum = msg1.msgnum;
        msgsnd(qid, &msg1, 3 * sizeof(int) + 512, 0);
   
   	printf("id=%d, talkers=%d, msg#=%d\n", id, msg1.cnum, msgnum);
   	while(1){
                scanf("%s", temp);

      		msgrcv(qid, &msg1, 3 * sizeof(int) + 512, INT_MAX, 0);
      		cnum = msg1.cnum;
      		msgnum = msg1.msgnum++;
      		msgsnd(qid, &msg1, 3 * sizeof(int) + 512, 0);
      		if(cnum == 1)
			printf("id=%d, talkers=%d, msg#=%d\n", id, cnum, msgnum);
      	
      		msg1 = nmessage(index, id, temp, msgnum);
                  
      		for(i = 0; i < cnum; ++i)
			msgsnd(qid, &msg1, 3 * sizeof(int) + 512, 0);

		if (strcmp(temp, "talk_quit")==0)
			break;
      		index++;
        }

        exit(0);
}

void do_receiver(int qid, int id, int index){
        struct q_entry msg1;

        while(1){
                msgrcv(qid, &msg1, 3 * sizeof(int) + 512, -index, 0);
                if (msg1.s_id!=id){
                        printf("[sender=%d & msg#=%d] %s\n", msg1.s_id, msg1.msgnum, msg1.msg);
                }
		else{
         		if(strcmp(msg1.msg, "talk_quit") == 0) 
                      		break;
      		}
      		index++;
        }

   	exit(0);
}

int main(int argc, char** argv){
        int i, qid, id, index;
        key_t key;
        struct q_entry msg1, msg2;
   	pid_t pid[2];
   	char tmp[512];

        key=ftok("key", 5);
   	qid = msgget(key, 0600 | IPC_CREAT | IPC_EXCL);

        if(qid == -1){
                qid = msgget(key, 0600);
	}
	else {
                msg1 = cmessage(INT_MAX, 0, 2);
                msgsnd(qid, &msg1, 3 * sizeof(int) + 512, 0);
        }

   	id=atoi(argv[1]);
        index=1;

   	msgrcv(qid, &msg1, 3 * sizeof(int) + 512, INT_MAX, 0);
   	msg1.cnum++;
   	msgsnd(qid, &msg1, 3 * sizeof(int) + 512, 0);

	pid[0] = fork();
	if (pid[0] == 0) {
		do_sender(qid, id);
		exit(0);
	}
	pid[1] = fork();
	if (pid[1] == 0) {
		do_receiver(qid, id, msg1.msgnum);
		exit(0);
	}
   	for(i = 0; i < 2; ++i)
		wait(0);
        
        msgrcv(qid, &msg2, 3 * sizeof(int) + 512, INT_MAX, 0);
        msg2.cnum--;
        msgsnd(qid, &msg2, 3 * sizeof(int) + 512, 0);

   	if(msg2.cnum == 0)
      		msgctl(qid, IPC_RMID, NULL);
        exit(0);
}

struct q_entry cmessage(int mtype, int cnum, int msgnum){
   	struct q_entry msg;
   
   	msg.mtype=mtype;
   	msg.cnum=cnum;
   	msg.s_id=0;
	msg.msgnum = msgnum;
   	strcpy(msg.msg, "");

   	return msg;
}

struct q_entry nmessage(int mtype, int s_id, char *str, int msgnum){
   	struct q_entry msg;
   
   	msg.mtype=mtype;
   	msg.cnum=0;
   	msg.s_id=s_id;
	msg.msgnum = msgnum;
   	strcpy(msg.msg, str);

   	return msg;
}

