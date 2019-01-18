#include "socklib.c"
#include "sys/types.h"
#include "sys/stat.h"

int main(int ac,char **av){
	int sock,fd;
	FILE *fpin;
	char request[BUFSIZ];

	if(ac!=2){
		printf("usage:ws [portnum]\n");
		return 1;
	}

	sock=make_server_socket(atoi(av[1]));
	if(sock==-1){
		perror("socket");
		return 1;
	}
	
	while(1){
		fd=accept(sock,NULL,NULL);
		if(fd==-1){
			perror("accept");
			return 1;
		}

	
		fpin=fopen(fd,"r");
		if(fpin==NULL){
			return 1;
		}

		fgets(request,BUFSIZ,fpin);
		printf("get a request:%s",request);
		read_til_crnl(fpin);
		process_rq(request,fd);
		fclose(fpin);
	}
}
// sock,fd,fpin,request,read_til_crnl,process_rq
