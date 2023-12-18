struct q_entry{
        long mtype;
	int snum;
        int cnum;
        int s_id;
        char msg[512];
};

struct q_entry cmessage(int mtype, int snum, int cnum);
struct q_entry nmessage(int mtype, int s_id, char *str);

void do_writer(int qid, int id){
	char temp[512];
        int i, cnum, index;
        struct q_entry msg1, msg2;

        while(1){
                scanf("%s", temp);
		 // (i) message 보내기 전 준비

		msg2=nmessage(index, id, temp);
               	for (i=0; i<cnum; i++){ 
        		 // (a) message 보내기
			sleep(1);
               	}

		if (cnum==1)
			printf("id=%d, talkers=%d, msg#=%d ...\n", id, msg1.cnum, msg1.snum);

                if (strcmp(temp, "talk_quit")==0)
                        break;
        }

        exit(0);
}

void do_reader(int qid, int id, int index){
        struct q_entry msg;

        while(1){
                // (j) message 받기
                if (msg.s_id!=id){
                        printf("[sender=%d & msg#=%d] %s\n", msg.s_id, msg.mtype, msg.msg);
                }
		// (c) message 받은 후 필요한 작업
                index++;
        }

	exit(0);
}

int main(int argc, char** argv){
        int i, qid, id, index;
	pid_t pid[2];
        key_t key;
        struct q_entry msg1, msg2;

        key=ftok("key", 5);
        // (k) message queue 만들고 초기화 작업

	id=atoi(argv[1]);
        // (l) 통신 전 필요한 작업
	printf("id=%d, talkers=%d, msg#=%d ...\n", id, msg1.cnum, msg1.snum);

	// (d) 함수 호출해서 message 주고 받기

        // (h) message 통신 완료 후 message queue 지우기

        exit(0);
}

struct q_entry cmessage(int mtype, int snum, int cnum){
	struct q_entry msg;
	
	msg.mtype=mtype;
	msg.snum=snum;
	msg.cnum=cnum;
	msg.s_id=0;
	strcpy(msg.msg, "");

	return msg;
}

struct q_entry nmessage(int mtype, int s_id, char *str){
	struct q_entry msg;
	
	msg.mtype=mtype;
	msg.snum=0;
	msg.cnum=0;
	msg.s_id=s_id;
	strcpy(msg.msg, str);

	return msg;
}
