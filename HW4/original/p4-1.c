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
                if (msg1.s_id!=id){
                        printf("[sender=%d & msg#=%d] %s\n", msg1.s_id, msg1.mtype, msg1.msg);
                }
		// (c) message 받은 후 필요한 작업
		index++;
        }

	exit(0);
}

int main(int argc, char** argv){
        int i, qid, id, index;
	key_t key;
	// 필요한 변수는 추가 선언
       
        key=ftok("key", 5);
        qid=msgget(key, 0600|IPC_CREAT);

	id=atoi(argv[1]);
	index=1;

	// (d) 함수 호출해서 message 주고 받기

        exit(0);
}

struct q_entry nmessage(int mtype, int s_id, char *str){
	struct q_entry msg;
	
	msg.mtype=mtype;
	msg.s_id=s_id;
	strcpy(msg.msg, str);

	return msg;
}
