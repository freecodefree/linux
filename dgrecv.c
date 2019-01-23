#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "dgram.c"

#define oops(m,x){perror(m);exit(x);}
void reply_to_sender(int,char *,struct sockaddr_in *,socklen_t);
void say_who_called(struct sockaddr_in *);

int main(int ac,char **av){
	int sock,portnum;
	char buf[BUFSIZ];
	size_t msglen;
	struct sockaddr_in saddr;
	socklen_t saddrlen;

	if(ac!=2||(portnum=atoi(av[1]))<=0){
		printf("usage:dgrecv portnum\n");
		return 1;
	}

	if((sock=make_dgram_server_socket(portnum))==-1){
		oops("make server socket",1);
	}
	
	saddrlen=sizeof(saddr);
	while((msglen=recvfrom(sock,buf,BUFSIZ,0,(struct sockaddr *)&saddr,&saddrlen))>0){
		buf[msglen]='\0';
		printf("dgrecv,got a message:%s\n",buf);
		say_who_called(&saddr);
		reply_to_sender(sock,buf,&saddr,saddrlen);
	}
	return 0;
}

void say_who_called(struct sockaddr_in *addrp){
	int port;
	char host[BUFSIZ];
	get_internet_address(host,BUFSIZ,&port,addrp);
	printf("got a message from %s:%d\n",host,port);
}

void reply_to_sender(int sock,char *msg,struct sockaddr_in *addrp,socklen_t len){
	char buf[BUFSIZ+BUFSIZ];
	sprintf(buf,"Thank you for your %d char message\n",strlen(msg));
	sendto(sock,buf,strlen(buf),0,(struct sockaddr *)addrp,len);
}
// msg,sockaddr_in,addrp,socklen_t,len,reply,sendto,make_dgram_server_socket
// size_t,msglen,sockaddr_in,socklen_t,saddrlen,recvfrom,say_who_called,
// reply_to_sender
