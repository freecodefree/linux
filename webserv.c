#include "socklib.c"
#include "sys/types.h"
#include "sys/stat.h"
#include "stdlib.h"
#include "string.h"

#define t(where,msg,i) {printf(where);printf(msg);printf("%d\n",i);}
void read_til_crnl(FILE *);
void process_rq(char *,int);
void header(FILE *,char *);
void cannot_do(int);
void do_404(char *,int);
int isadir(char *);
int not_exist(char *);
void do_ls(char *,int);
char *file_type(char *f);
int ends_in_cgi(char *);
void do_exec(char *,int);
void do_cat(char *,int);


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

	
		fpin=fdopen(fd,"r");
		if(fpin==NULL){
			return 1;
		}

		fgets(request,BUFSIZ,fpin);
		printf("get a request:%s\n",request);
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
//	t("process_rq:",NULL,0);
	char cmd[11],arg[513];
	if(fork()!=0)return;
	strcpy(arg,".");
	if(sscanf(rq,"%s%s",cmd,(arg+1))!=2);{
		printf("dbg:get cmd&arg:%s&%s\n",cmd,arg);
//		return;
	}
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

void header(FILE *fp,char *content_type){
	fprintf(fp,"HTTP/1.0 200 OK\r\n");
	if(content_type){
		fprintf(fp,"Content-type:%s\r\n",content_type);
	}
}

void cannot_do(int fd){
	FILE *fp=fdopen(fd,"w");
	fprintf(fp,"HTTP/1.0 501 Not Implemented\r\n");
	fprintf(fp,"Content-type: text/plain\r\n");
	fprintf(fp,"\r\n");

	fprintf(fp,"That command is not yet implemented\r\n");
	fclose(fp);
}

void do_404(char *item,int fd){
	FILE *fp=fdopen(fd,"w");

	fprintf(fp,"HTTP/1.0 404 Not Found\r\n");
	fprintf(fp,"Content-type: text/plain\r\n");
	fprintf(fp,"\r\n");

	fprintf(fp,"The item you requested: %s\r\nis not found\r\n",item);
	fclose(fp);
}

int isadir(char *f){
	struct stat info;
	int a,b;
	a=stat(f,&info);
	b=S_ISDIR(info.st_mode);
	t("isdir:","stat()=",a);
	t("isdir:","S_ISDIR()",b);
	return ((a!=-1)&&b);
}

int not_exist(char *f){
	struct stat info;
	return (stat(f,&info)==-1);
}

void do_ls(char *dir,int fd){
	FILE *fp=fdopen(fd,"w");
	header(fp,"test/plain");
	fprintf(fp,"\r\n");
	fflush(fp);
	
	t("do_ls:dir=",dir,fd);
	dup2(fd,1);
	dup2(fd,2);
	close(fd);
	t("do_ls:dir=",dir,fd);
	execlp("ls","ls","-l",dir,NULL);
	perror(dir);
	exit(1);
}

char *file_type(char *f){
	char *cp;
	if((cp=strchr(f,'.'))!=NULL){
		return cp+1;
	}
	return "";
}
int ends_in_cgi(char *f){
	return (strcmp(file_type(f),"cgi")==0);
}

void do_exec(char *prog,int fd){
	FILE *fp=fdopen(fd,"w");
	header(fp,NULL);
	fflush(fp);

	dup2(fd,1);
	dup2(fd,2);
	close(fd);
	execl(prog,prog,NULL);
	perror(prog);
}

void do_cat(char *f,int fd){
	char *extension;
	char *content="text/plain";
	int c;
	FILE *fpsock,*fpfile;

	extension=file_type(f);
	if(strcmp(extension,"htlm")==0){
		content="text/html";
	}else if(strcmp(extension,"gif")==0){
		content="text/gif";
	}else if(strcmp(extension,"jpg")==0){
		content="text/jpg";
	}

	fpsock=fdopen(fd,"w");
	fpfile=fopen(f,"r");
	if(fpsock==NULL||fpfile==NULL){
		return;
	}
	header(fpsock,content);
	fprintf(fpsock,"\r\n");
	while((c=getc(fpfile))!=EOF){
		putc(c,fpsock);
	}
	fclose(fpsock);
	fclose(fpfile);
	return;
	
}
// extension,content,fpsock,fpfile,
// file_type,strrchr,prog,execl
// dir,fd,header,text/plain,fflush,dup2
// stat,info,stat,S_ISDIR,st_mode, 
// rq,fd,cmd,arg,cannot_do,not_exist,do_404,isadir,do_ls,ends_in_cgi,do_exec
// do_cat
// sock,fd,fpin,request,read_til_crnl,process_rq
