#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "netdb.h"
#include "time.h"
#include "strings.h"

#define PORTNUM 13000
#define HOSTLEN 256
#define oops(msg) {perror(msg);exit(1);}

int main(){
	struct sockaddr_in saddr;
	struct hostent *hp;
	char hostname[HOSTLEN];
	int sock_id,sock_fd;
	FILE *sock_fp;
	time_t thetime;
// step 1
	sock_id=socket(PF_INET,SOCK_STREAM,0);
	if(sock_id==-1){
		oops("socket");
	}
// step 2
	bzero((void *)&saddr,sizeof(saddr));
	gethostname(hostname,HOSTLEN);
	hp=gethostbyname(hostname);
	bcopy((void *)hp->h_addr,(void *)&saddr.sin_addr,hp->h_length);
	saddr.sin_port=htons(PORTNUM);
	saddr.sin_family=AF_INET;
	if(bind(sock_id,(struct sockaddr *)&saddr,sizeof(saddr))!=0){
		oops("bind");
	}
// step 3
	if(listen(sock_id,1)!=0){
		oops("listen");
	}

	while(1){
		sock_fd=accept(sock_id,NULL,NULL);
		printf("get a call\n");
		if(sock_fd==-1){
			oops("accept");
		}
		sock_fp=fdopen(sock_fd,"w");
		if(sock_fp==NULL){
			oops("fdopen");
		}
		thetime=time(NULL);
		fprintf(sock_fp,"time:%s",ctime(&thetime));
		fclose(sock_fp);
	}
}
// listen,accept,fdopen,time,
// gethostname,gethostbyname,bcopy,h_addr,sin_addr,h_length,sin_port,htons
// sin_family,AF_INET,bind,
// sockaddr_in,saddr,hostent,hp,hostname,sock_id,sock_fd,sock_fp,ctime
// time_t,thetime,socket,PF_INET,SOCK_STREAM,bzero,
