#include "stdio.h"
#include "pthread.h"
#include "stdlib.h"
#include "unistd.h"

#define NUM 5
int main (){
	pthread_t t1,t2;
	void *print_msg(void *);

	pthread_create(&t1,NULL,print_msg,(void *)"hello");
	pthread_create(&t2,NULL,print_msg,(void *)"world\n");
//	pthread_join(t1,NULL);
//	pthread_join(t2,NULL);
	printf("done");
	return 0;
}

void *print_msg(void *msg){
	for(int i=0;i<NUM;i++){
		printf("%s",(char *)msg);
		fflush(stdout);
		sleep(1);
	}
	return NULL;
}
// pthread_t,print_msg,pthread_create,pthread_join
