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

#define R 4
#define B 10

struct databuf1 {
	int s_id;
	char msg[512];
};

struct databuf2 {
	int index;
	int members[R];
	struct databuf1 buf[B];
};

union semun {
	int val;
	struct semid_ds *buf;
	ushort *array;
};

void sem_wait (int semid, int semidx) {
	struct sembuf p_buf = {semidx, -1, 0};

	if (semop(semid, &p_buf, 1) == -1)
		printf("semwait fails...\n");
}

void sem_signal (int semid, int semidx) {
	struct sembuf p_buf = {semidx, 1, 0};

	if (semop(semid, &p_buf, 1) == -1)
		printf("semsignal fails...\n");
}

void do_reader(int id, int rindex, struct databuf2 *buf, int semid) {
	int i;
	for(i=rindex; ; i=(i+1)%B) {
		sem_wait(semid, id);
		for (i = 1; i <= 4; i++) {
			if (id == i)
				sleep(i);
		}
		
		if (buf->buf[rindex].s_id != id)
			printf("[sender=%d & msg#=%d] %s\n", buf->buf[rindex].s_id, rindex, buf->buf[rindex].msg);

		sem_signal(semid, id + 4);

		if (buf->buf[rindex].s_id == id) {
			if (strcmp(buf->buf[rindex].msg, "talk_quit") == 0)
				break;
		}
		
		rindex = (rindex + 1) % 10;
	}
	
	exit(0);
}

void do_writer(int id, struct databuf2 *buf, int semid) {
	/* TODO */
	int i, cnum, index;
	char temp[512];
	while(1) {
		scanf("%s", temp);
		
		//sem_wait(semid, 0);
		
		cnum = 0;
		for (i = 0; i < 4; i++) {
			if (buf->members[i] == 1) {
				sem_wait(semid, i + 5);
				cnum++;
			}
		}

		sem_wait(semid, 0);

		index = buf->index;
		buf->buf[index].s_id = id;
		strcpy(buf->buf[index].msg, temp);
		buf->index = (buf->index + 1) % 10;
		
		sem_signal(semid, 0);

		for (i = 0; i < 4; i++) {
			if (buf->members[i] == 1)
				sem_signal(semid, i + 1);
		}
		
		if (cnum == 1)
			printf("id=%d, talkers=%d, msg#=%d...\n", id, cnum, buf->index);
		
		//sem_signal(semid, 0);
		
		if (strcmp(temp, "talk_quit") == 0)
			break;	
	}

	exit(0);
}

int main(int argc, char** argv) {
	int id, semid, shmid, i, j, flag, cnum, rindex;
	ushort init[9] =  {1, 0, 0, 0, 0, 10, 10, 10, 10};
	key_t key1, key2;
	pid_t pid[2];
	union semun arg;
	struct databuf2 *buf;

	key1 = ftok("key", 3);
	if ((semid = semget(key1, 9, 0600|IPC_CREAT|IPC_EXCL)) > 0) {
		arg.array = init;
		semctl(semid, 0, SETALL, arg);
	}
	else {
		semid = semget(key1, 9, 0);
	}
		
	key2 = ftok("key", 5);
	shmid = shmget(key2, sizeof(struct databuf2), 0600|IPC_CREAT);
	buf = (struct databuf2 *)shmat(shmid, 0, 0);

	id = atoi(argv[1]);

	sem_wait(semid, 0);
	
	//읽기, 쓰기 하기 전에 필요한 작업
	if (id > 4 || buf->members[id - 1] != 0)
		id = -1;
	else { 
		cnum = 0;
		rindex = buf->index;
		buf->members[id - 1] = 1;
		for (i = 0; i < 4; i++) {
			if (buf->members[i] == 1)
				cnum++;
		}
	}			

	sem_signal(semid, 0);
	
	if (id == -1)
		return 0;
	
	printf("id=%d, talkers=%d, msg#=%d...\n", id, cnum, rindex);
	
	for (i = 0; i < 2; i++) {
		pid[i] = fork();
		if (pid[i] == 0 && i == 0)
			do_reader(id, rindex, buf, semid);
		else if (pid[i] == 0 & i == 1)
			do_writer(id, buf, semid);
	}

	for (i = 0; i < 2; i++) {
		wait(0);
	}

	sem_wait(semid, 0);
	
	//종료할 때 필요한 작업, 마지막으로 종료하는 talker이면, flag = 0으로 설정
	buf->members[id - 1] = 0;
	cnum = 0;
	for (i = 0; i < 4; i++) {
		if (buf->members[i] == 1)
			cnum++;
	}
	
	if (cnum == 0)
		flag = 0;
	else
		flag = 1;
	
	sem_signal(semid, 0);
		
	if (flag == 0) {
		semctl(semid, IPC_RMID, 0);
		shmctl(shmid, IPC_RMID, 0);
		printf("sem & shm IPC_RMID...\n");
	}

	exit(0);
}
