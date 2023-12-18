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
	 필요한 변수 추가
        struct q_entry msg1, msg2;

        key=ftok("key", 5);
        // (f) message queue 만들고 초기화 작업

	id=atoi(argv[1]);
	index=1;

        // (g) message 통신 전 필요한 작업

	// (d) 함수 호출해서 message 주고 받기

        // (h) message 통신 완료 후 message queue 지우기

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
