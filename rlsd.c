#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "netdb.h"
#include "time.h"
#include "strings.h"
#include "ctype.h"
#include "stdlib.h"

#define PORTNUM 13001
#define HOSTLEN 256
#define oops(msg) {perror(msg);exit(1);}

void sanitize(char *str){
	char *src,*dest;

	for(src=dest=str;*src;src++){
		if(*src=='/'||isalnum(*src)){
			*dest++=*src;
		}
	}
	*dest='\0';
}

int main(int ac,char **av){
	struct sockaddr_in saddr;
	struct hostent *hp;
	char hostname[HOSTLEN];
	int sock_id,sock_fd;
	FILE *sock_fpi,*sock_fpo;
	FILE *pipe_fp;
	char dirname[BUFSIZ],command[BUFSIZ];
	int dirlen,c;

	sock_id=socket(AF_INET,SOCK_STREAM,0);
	if(sock_id==-1){
		oops("socket");
	}

	bzero(&saddr,sizeof(saddr));
	gethostname(hostname,HOSTLEN);
	hp=gethostbyname(hostname);
	bcopy(hp->h_addr,&saddr.sin_addr,hp->h_length);
	saddr.sin_port=htons(PORTNUM);
	saddr.sin_family=AF_INET;
	if(bind(sock_id,(struct sockaddr *)&saddr,sizeof(saddr))!=0){
		oops("bind");
	}

	if(listen(sock_id,1)!=0){
		oops("listen");
	}

	while(1){
		sock_fd=accept(sock_id,NULL,NULL);
		if(sock_fd==-1){
			oops("accept");
		}
		printf("accept\n");
		sock_fpi=fdopen(sock_fd,"r");
		if(sock_fpi==NULL){
			oops("fdopen reading");
		}
		if(fgets(dirname,BUFSIZ-5,sock_fpi)==NULL){
			oops("fgets");
		}
		sanitize(dirname);
		printf("get dirname:%s\n",dirname);
		
		sprintf(command,"ls %s",dirname);
		if((pipe_fp=popen(command,"r"))==NULL){
			oops("popen");
		}
		sock_fpo=fdopen(sock_fd,"w");
		if(sock_fpo==NULL){
			oops("fdopen writing");
		}

		while((c=getc(pipe_fp))!=EOF){
			putc(c,stdout);
			putc(c,sock_fpo);
		}
		putc(c,sock_fpo);
		write(sock_id,"done\n",5);
		printf("message sent\n");
		sleep(2);
		fclose(sock_fpi);
		fclose(sock_fpo);
		pclose(pipe_fp);
	}
}
// fdopen,fgets,sanitize,
// sockaddr_in,saddr,hostent,hp,hostname,sock_fpi/o,pipe_fp,dirname,command,dirname
