#include<stdio.h>
#include<stdlib.h>

struct st{
	int id;
	char name[30];
};

void add_five(struct st *data) {
	int i;
	for(i=0;i<5;i++){
		(data+i)->id+=5;
	}
}

int main(void){
	FILE *fp;
	int i;
	struct st *data;
	data=malloc(sizeof(struct st) * 5);
	fp=fopen("data", "w");
	for(i=0;i<5;i++){
		scanf("%d", &data[i].id);
		scanf("%s", &data[i].name);
	}
	add_five(data);
	for(i=0;i<5;i++){

		fprintf(fp, "%-5d", data[i].id);
		fprintf(fp, "%-5s\n", data[i].name);
	}

	return 0;
}
