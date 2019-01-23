#include "stdio.h"
#include "stdlib.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "dgram.c"

#define oops(m,x){perror(m);exit(x);}

int main(int ac,char **av){
	int sock;
	char *msg;
	int port;
	struct sockaddr_in saddr;
	size_t msglen;
	socklen_t saddrlen;
	char buf[BUFSIZ];

	if(ac!=4){
		fprintf(stderr,"usage:dgsend host port message\n");
		return 1;
	}

	msg=av[3];
	port=atoi(av[2]);

	if((sock=make_dgram_client_socket())==-1){
		oops("cannot get a client socket",1);
	}

	make_internet_address(av[1],port,&saddr);

	if(sendto(sock,msg,strlen(msg),0,(struct sockaddr *)&saddr,sizeof(saddr))==-1){
		oops("cannot sendto",1);
	}

	while((msglen=recvfrom(sock,buf,BUFSIZ,0,NULL,NULL))>0){
		buf[msglen]='\0';
		printf("dgsend,get a message:%s\n",buf);
		sendto(sock,"Not at all",10,0,(struct sockaddr *)&saddr,sizeof(saddr));
	}
	return 0;
}
// 
