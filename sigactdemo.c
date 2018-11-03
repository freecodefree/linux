#include "stdio.h"
#include "signal.h"
#include "stdlib.h"
#include "unistd.h"
#define INPUTLEN 100

void intHandler(int);

int main(){
	struct sigaction newHandler;
	char x[INPUTLEN];
	sigset_t blocked;

	newHandler.sa_handler=intHandler;
	newHandler.sa_flags=SA_RESETHAND;

	sigemptyset(&(blocked));
	sigaddset(&(blocked),SIGQUIT);
	newHandler.sa_mask=blocked;

	if(sigaction(SIGINT,&newHandler,NULL)==-1){
		perror("sigaction");
	}else{
		while(1){
			read(0,&x,INPUTLEN-1);
			x[INPUTLEN]='\0';
			printf("input:%s\n",x);
		}
	}
	return 0;

}

void intHandler(int s){
	printf("received an SIGINT(%d)\n",s);
	sleep(2);
	printf("leaving intHandler\n");
}
// sigaction newHandler,sigset_t,blocked,intHandler,sa_handler,sa_flags
// SA_RESETHAND,sigemptyset,sigaddset,sa_mask,
