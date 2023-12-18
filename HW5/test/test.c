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

void sem_wait(int semid, int semidx){
   struct sembuf p_buf={semidx,-1,0};
   if(semop(semid,&p_buf,1)==-1)
      printf("semWait fails..\n");
}
void sem_signal(int semid, int semidx){
        struct sembuf p_buf={semidx,1,0};
        if(semop(semid,&p_buf,1)==-1)
                printf("semSignal fails..\n");
}

union semun{
   int val;
   struct semid_ds *buf;
   unsigned short *array;   
};
struct databuf1{
        int s_id; //보낸 사람의 id
        char msg[512];
};
struct databuf2{
   int index; //버퍼에서 데이터를 쓸 위치(index)
   int members[4]; //각 클라이언트의 접속 정보 0/1 .
   struct databuf1 buf[10];
};

void do_writer(int id,  struct databuf2 *buf, int semid){
   char temp[512];
        int i, cnum, index; //내가 이번에 보낼 번호

   do{
	scanf("%s", temp);
   
      sem_wait(semid,0); // semWait(s);
      
      cnum=0; //현재 참여자 인원 파악용
   
      for(i=0;i<4;i++){
         if(buf->members[i]!=0){ //참여자라면
            sem_wait(semid, i+4+1); //semWait(e);
            cnum++;
         }
      }

      index = buf->index; //보낼 인덱스
      buf->buf[index].s_id = id; //id 설정
      strcpy(buf->buf[index].msg, temp); //메세지 복사                                           
      buf->index = (buf->index+1)%10; //index 늘리기
      
      for(i=0;i<4;i++){
                        if(buf->members[i]!=0){ //참여자라면
                                sem_signal(semid, i+1); // semSignal(n);
                        }
                }
      
      if(cnum==1)                                                                      
                         printf("id=%d, talkers=%d, msg#=%d ...\n", id, 1, buf->index);    
      
      sem_signal(semid,0); //semSignal(s);
      
      
        }while(strcmp(temp,"talk_quit"));

        exit(0);
}

void do_reader(int id, int rindex, struct databuf2 *buf, int semid){
        union semun arg;

        while(1){
      
      sem_wait(semid,id); // semWait(n);
      if(id==1)
         sleep(2);

      if(buf->buf[rindex].s_id != id){
         printf("[sender=%d & msg#=%d] %s\n", buf->buf[rindex].s_id, rindex, buf->buf[rindex].msg);
      }
   

      sem_signal(semid,id+4); //semSignal(e);

      if(buf->buf[rindex].s_id == id &&  strcmp(buf->buf[rindex].msg,"talk_quit")==0){
         break; //sem안으로?
      }
         
      rindex=(rindex+1)%10;
   }        

   exit(0);
}
int semid,shmid;
void catch_int(int signo){
   shmctl(shmid,IPC_RMID,0);
   semctl(semid,0,IPC_RMID,0);
   exit(0);
}

int main(int argc, char** argv){
   static struct sigaction act;
   act.sa_handler = catch_int;
   sigaction(SIGINT,&act,NULL);

        int i,j, id, rindex, flag=1, cnum;
        key_t key1, key2;
   pid_t pid[2];
   ushort init[9] = {1,0,0,0,0,10,10,10,10}; // s n e
   union semun arg;
   struct databuf2 *buf;
        
   key1=ftok("key",3);
   semid=semget(key1,9,0600|IPC_CREAT|IPC_EXCL);
   if(semid==-1){      
      semid=semget(key1,9,0);
                printf("세마포존재 ㅠ\n");
   }
   else{
      arg.array=init;
                semctl(semid, 0, SETALL, arg);
                printf("세마포 초기화!\n");
   }


   key2=ftok("key",5);
   shmid = shmget(key2,sizeof(struct databuf2),0600|IPC_CREAT);
   buf = (struct databuf2*)shmat(shmid,0,0);
   
   id=atoi(argv[1]);

   int tmp;

   sem_wait(semid,0); //semWait(s);
   if(buf->members[id-1]!=0){
      printf("참여 안됩니다!\n");
      id=-1;
   }
   else{   
      cnum=0;
      rindex=buf->index;
      buf->members[id-1] = 2; //읽을수 있다!
      for(i=0;i<4;i++){
         if(buf->members[i]!=0)
            cnum++;
      }
   }
   sem_signal(semid,0); //semSignal(s);

   if(id==-1){
      shmctl(shmid,IPC_RMID,0);
                semctl(semid,0,IPC_RMID,0);
      return 0;
   }

   printf("id=%d, talkers=%d, msg#=%d...\n",id,cnum,rindex);
   
   for(i=0;i<2;i++){
      pid[i]=fork();
      if(pid[i]==0 && i==0){
         do_reader(id,rindex,buf,semid);
         exit(0);
      }
      else if(pid[i]==0 && i==1){
         do_writer(id,buf,semid);
         exit(0);
      }
   }

   wait(0);wait(0);   

   
        sem_wait(semid,0); //semWait(s);
   buf->members[id-1]=0;
   cnum=0;
        for(i=0;i<4;i++){
                if(buf->members[i]!=0)
                                cnum++;
                
        }
   if(cnum==0){   
      flag=0;
   }   

        sem_signal(semid,0); //semSignal(s);

   if(flag==0){
      shmctl(shmid,IPC_RMID,0);
           semctl(semid,0,IPC_RMID,0);
      printf("sem shm 종료다 이자식아\n");
   }   



        exit(0);
}
