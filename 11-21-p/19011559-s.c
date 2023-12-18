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

struct q_entry {
	long mtype;
	int data;
};

int main(void) {
	int i, qid;
	key_t key;
	struct q_entry msg;

	key = ftok("key", 3);
	qid = msgget(key, IPC_CREAT|0600);

	for (i = 0; i < 15; i++) {
		msgrcv(qid, &msg, sizeof(int), -3, 0);
		msg.mtype += 3;
		msg.data += 8;
		msgsnd(qid, &msg, sizeof(int), 0);
	}

	exit(0);
}
