#include "stdio.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "netdb.h"
#include "stdlib.h"
#include "strings.h"
#include "unistd.h"

#define oops(msg) {perror(msg);exit(1);}
#define PORTNUM 1500

int main(int ac,char **av){
	struct sockaddr_in servadd;
	struct hostent *hp;
	int sock_id,sock_fd;
	char buffer[BUFSIZ];
	int n_read;

	if(ac!=3){
		printf("usage:hostname portnum");
		exit(1);
	}

	sock_id=socket(AF_INET,SOCK_STREAM,0);
	if(sock_id==-1){
		oops("socket");
	}

	bzero(&servadd,sizeof(servadd));
	hp=gethostbyname(av[1]);
	if(hp==NULL){
		oops("hostname");
	}
	bcopy(hp->h_addr,&servadd.sin_addr,hp->h_length);
	servadd.sin_port=htons(PORTNUM);
	servadd.sin_family=AF_INET;
	if(connect(sock_id,(struct sockaddr *)&servadd,sizeof(servadd))!=0){
		oops("connect");
	}

	if(write(sock_id,av[2],sizeof(av[2]))==-1){
		oops("write1");
	}
	if(write(sock_id,"\n",1)==-1){
                oops("write2");
        }

	while((n_read=read(sock_id,buffer,BUFSIZ))>0){
		if(write(1,buffer,n_read)==-1){
			oops("write3");
		}
	}
	close(sock_id);

}
// 
// baero,gethostbyname,bcopy,sockaddr,sin_addr,h_length,sin_port,htons,sin_family
// sockaddr_in,servadd,hostent,hp,sock_id,sock_fd,buffer,n_read
