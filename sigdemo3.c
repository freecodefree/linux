#include "stdio.h"
#include "signal.h"
#include "string.h"
#include "unistd.h"
#include "fcntl.h"

#define INPUTLEN 100
void inthandler(int);
void quithandler(int);
int main(){
	char input[INPUTLEN];
	int nChars;
	signal(SIGINT,inthandler);
	signal(SIGQUIT,quithandler);
	do{
		nChars=read(0,input,(INPUTLEN-1));
		if(nChars==-1){
			perror("read");
		}else{
			input[INPUTLEN]='\0';
			printf("you have input:%s",input);
		}
	}while(strncmp(input,"quit",4)!=0);
	return 0;
}

void inthandler(int s){
	printf("received a int signal(%d)!\n",s);
	sleep(2);
	printf("leaving inthandler\n");
}
void quithandler(int s){
        printf("received a quit signal(%d)!\n",s);
        sleep(2);
        printf("leaving quithandler\n");
}

// nChars,SIGINT,SIGQUIT,input,strncmp
