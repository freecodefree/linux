#include "stdio.h"
#include "stdlib.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "netdb.h"
#include "unistd.h"
#include "strings.h"

#define oops(msg) {perror(msg);exit(1);}
int main(int ac,char **av){
	struct sockaddr_in servadd;
	struct hostent *hp;
	int sock_id,sock_fd;
	char message[BUFSIZ];
	int messlen;

	sock_id=socket(AF_INET,SOCK_STREAM,0);
	if(sock_id==-1){
		oops("socket");
	}

	bzero((void *)&servadd,sizeof(servadd));
	hp=gethostbyname(av[1]);
	if(hp==NULL){
		oops(av[1]);
	}
	bcopy((void *)hp->h_addr,(void *)&servadd.sin_addr,hp->h_length);
	servadd.sin_port=htons(atoi(av[2]));
	servadd.sin_family=AF_INET;
	if(connect(sock_id,(struct sockaddr *)&servadd,sizeof(servadd))!=0){
		oops("connect");
	}

	messlen=read(sock_id,message,BUFSIZ);
	if(messlen==-1){
		oops("read");
	}
	if(write(1,message,messlen)!=messlen){
		oops("write");
	}
	close(sock_id);
}
// messlen,
// bzero,gethostbyname,bcopy,h_addr,sockaddr,h_length,sin_port,htons
// sin_family,AF_INET,connect
// sockaddr_in,servadd,hostent,hp,sock_id,sock_fd,message,BUFSIZ,messlen
// socket,AF_INET,SOCK_STREAM,
