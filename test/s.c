#include "socklib.c"
#include "string.h"
//#include "c.c"
#define PORTNUM 13000

int main(){
	int sock,id,nread,n=0,ncall=0;
	char buf[BUFSIZ];
	sock=make_server_socket(PORTNUM);
	if(sock==-1){
		perror("make_server_socket");
		return 1;
	}
	
	while(1){
		if((id=accept(sock,NULL,NULL))==-1){
			perror("accept");
			return 1;
		}
		write(id,"hi welcome",13);
		printf("server:get a call(%d)\n",ncall++);
		while(1){
			nread=read(id,buf,BUFSIZ);
			if(nread<=0){
				close(id);
				n=0;
				break;
			}
//			write(id,"hello welcome",13);
//			write(id,"\n",1);
			buf[nread]='\0';
			printf("server:get your message(%d),%s",n,buf);
			sleep(2);
//			write(id,"hello welcome",13);
			sprintf(buf,"hello welcome(%d)",n++);
			write(id,buf,strlen(buf));
//			write(1,buf,nread);
//			putchar('\n');
		}
	}
}

