#define R 4
#define B 10

struct databuf1{
        int s_id;
        char msg[512];
};

struct databuf2{
        int index;
        int members[R];
        struct databuf1 buf[B];
};

union semun{
        int val;
        struct semid_ds *buf;
        ushort *array;
};

void sem_wait(int semid, int semidx){
        struct sembuf p_buf={semidx, -1, 0};

        if (semop(semid, &p_buf, 1)==-1)
		printf("semwait fails...\n");
}

void sem_signal(int semid, int semidx){
        struct sembuf p_buf={semidx, 1, 0};

        if (semop(semid, &p_buf, 1)==-1)
		printf("semsignal fails...\n");
}

void do_reader(int id, int rindex, struct databuf2 *buf, int semid){
        int i;

        for (i=rindex; ; i=(i+1)%B){
		if (id%2==1) sleep(id);
                // semaphore 작업
                if (/* 조건 */){
                        printf("[sender=%d & msg#=%d] %s\n", buf->buf[i].s_id, i, buf->buf[i].msg);
                }
                // semaphore 작업
                if (/* 조건 */)
                        break;
        }

        exit(0);
}

void do_writer(int id, struct databuf2 *buf, int semid){
        char temp[512];
        int i, flag=0, cnum;

        for ( ; ; ){
                scanf("%s", temp);
		cnum=0;
                // semaphore 작업
                // semaphore 작업
                // msg write
                // msg의 sender 설정
                // index update
		if (cnum==1)
			printf("id=%d, talkers=%d, msg#=%d...\n", id, cnum, buf->index);
                // semaphore 작업
		// semaphore 작업
                if (strcmp(temp, "talk_quit")==0)
			break;
        }

	exit(0);
}

int main(int argc, char** argv){
        int id, semid, shmid, i, j, flag, cnum, rindex;
        ushort /* semaphore 초기값 설정 */
	key_t key1, key2;
	pid_t pid[2];
        union semun arg;
        struct databuf2 *buf;

	key1=ftok("key", 3);
        if ((semid=semget(key1, /* semaphore 개수 */, 0600|IPC_CREAT|IPC_EXCL))>0){
                arg.array=init;
                semctl(semid, 0, SETALL, arg);
        }
        else{
                semid=semget(key1, /* semaphore 개수 */, 0);
        }

	key2=ftok("key", 5);
        shmid=shmget(key2, sizeof(struct databuf2), 0600|IPC_CREAT);
        buf=(struct databuf2 *)shmat(shmid,0,0);

	id=atoi(argv[1]);

        // semaphore 작업
	// 읽기, 쓰기 하기 전에 필요한 작업 
        // semaphore 작업

	if (id==-1)
		return 0;
	printf("id=%d, talkers=%d, msg#=%d...\n", id, cnum, rindex);

	for (i=0; i<2; i++){
		pid[i]=fork();
        	if (pid[i]==0 && i==0)
               		do_reader(id, rindex, buf, semid);
		else if (pid[i]==0 && i==1)
        		do_writer(id, buf, semid);
	}

	for (i=0; i<2; i++){
        	wait(0);
	}

        // semaphore 작업
        // 종료할 때 필요한 작업, 마지막으로 종료하는 talker이면, flag=0으로 설정
        // semaphore 작업
	if (flag==0){
		semctl(semid, IPC_RMID, 0);
		shmctl(shmid, IPC_RMID, 0);
		printf("sem & shm IPC_RMID...\n");
	}

        exit(0);
}
