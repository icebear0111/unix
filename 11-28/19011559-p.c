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

union semun{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

struct buffer {
	int f, r;
	char msg[10][512];
};

int main(void) {
	char temp[512];
	ushort buf[3] = {1, 0, 10};
	int i, fd, n, semid, shmid;
	key_t key1, key2;
	union semun arg;
	struct buffer *msg_buf;
	struct sembuf p_buf;
	
	key1 = ftok("key", 5);
	semid = semget(key1, 3, 0600 | IPC_CREAT | IPC_EXCL);
	if(semid == -1) {
		semid = semget(key1, 3, 0600);
	}
	else {
		arg.array = buf;
		semctl(semid, 0, SETALL, arg);	
	}

	key2 = ftok("key", 2);
	shmid = shmget(key2, sizeof(struct buffer), 0600 | IPC_CREAT);
	msg_buf = (struct buffer *)shmat(shmid, 0, 0);
	msg_buf->f=msg_buf->r = 0;

	do {
		scanf("%s", temp);
		p_buf.sem_num = 2;
		p_buf.sem_op = -1;
		p_buf.sem_flg = 0;
		semop(semid, &p_buf, 1);

		p_buf.sem_num = 0;
		p_buf.sem_op = -1;
		p_buf.sem_flg = 0;
		semop(semid, &p_buf, 1);
		
		//temp을 shared memory에 복사
		strcpy(msg_buf->msg[msg_buf->r], temp);
		printf("idx = %d : msg = %s\n", msg_buf->r, msg_buf->msg[msg_buf->r]);
		msg_buf->r = (msg_buf->r + 1) % 10;
		
		p_buf.sem_num = 0;
		p_buf.sem_op = 1;
		p_buf.sem_flg = 0;
		semop(semid, &p_buf, 1);

		p_buf.sem_num = 1;
		p_buf.sem_op = 1;
		p_buf.sem_flg = 0;
		semop(semid, &p_buf, 1);

	} while(strcmp(temp, "quit"));
		
				
	exit(0);
}
