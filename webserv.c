#include "socklib.c"
#include "sys/types.h"
#include "sys/stat.h"
#include "string.h"

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

void read_til_crnl(FILE *fp){
	char buf[BUFSIZ];
	while(fgets(buf,BUFSIZ,fp)!=NULL&&strcmp(buf,"\r\n")!=0);
}

void process_rq(char *rq,int fd){
	char cmd[11],arg[513];
	if(fork()!=0)return;
	strcpy(arg,"./");
	if(scanf(rq,"%10s%512s",cmd,(arg+2))!=2)return;
	if(strcmp(cmd,"GET")!=0){
		cannot_do(fd);
	}else if(not_exist(arg)){
		do_404(arg,fd);
	}else if(isadir(arg)){
		do_ls(arg,fd);
	}else if(ends_in_cgi(arg)){
		do_exec(arg,fd);
	}else{
		do_cat(arg,fd);
	}
}
// rq,fd,cmd,arg,cannot_do,not_exist,do_404,isadir,do_ls,ends_in_cgi,do_exec
// do_cat
// sock,fd,fpin,request,read_til_crnl,process_rq
