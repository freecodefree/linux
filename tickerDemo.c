#include "stdio.h"
#include "sys/time.h"
#include "signal.h"
#include "stdlib.h"
void countDown(int);
int setTicker(int);

int main(){
	signal(SIGALRM,countDown);
	if(setTicker(500)==-1){
		perror("setTicker");
	}else{
		while(1);
	}
	return 1;
}

void countDown(int signum){
	static int i=10;
	printf("%d\n",i--);
	if(i==0){
		printf("Done!");
		exit(0);
	}
}

int setTicker(int nMsecs){
	struct itimerval newTimeset;
	long nSec,nUsecs;
	nSec=nMsecs/1000;
	nUsecs=(nMsecs%1000)*1000L;

	newTimeset.it_interval.tv_sec=nSec;
	newTimeset.it_interval.tv_usec=nUsecs;

	newTimeset.it_value.tv_sec=nSec+5L;
	newTimeset.it_value.tv_usec=nUsecs;

	return setitimer(ITIMER_REAL,&newTimeset,NULL);
}
// nMsecs,itimerval,newTimeset,nSec,nUsecs,it_interval,it_value,tv_sec,tv_usec
// setitimer,ITIMER_REAL
// countDown,SIGALRM,setTicker
