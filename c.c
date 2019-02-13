#include "stdio.h"
#include "socklib.c"
//#include "unistd.h"
#define PORTNUM 13000

//int connect_to_server(char *,int);

int main(){
	int id,nread;
	id=connect_to_server("shenzhou",PORTNUM);
	if(id==-1){
		perror("connect_to_server");
		return 1;
	}
	char buf[BUFSIZ];
	sleep(1);
	write(id,"hello",5);
	write(id,"\n",1);
	while(1){
//		printf("input('q' to quit):");
//		fflush(stdout);
		write(1,">",1);
		nread=read(0,buf,BUFSIZ);
		if(*buf=='q'){
			close(id);
			return 0;
		}
		write(id,buf,nread);
//		write(id,"\n",1);
		nread=read(id,buf,BUFSIZ);
		buf[nread]='\0';
		printf("from server:%s\n",buf);
//		write(1,"from server:",12);
//		write(1,buf,nread);
//		putchar('\n');
	}
}
