#include "stdio.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "string.h"
#include "pthread.h"
#include "stdlib.h"
#include "unistd.h"
#include "dirent.h"
#include "time.h"
#include "socklib.c"

time_t server_stated;
int server_bytes_sent;
int server_requests;

int main(int ac,char **av){
	int fd,sock;
	int *fdptr;
	pthread_t worker;
	pthread_attr_t attr;

	if(ac!=2){
		printf("usage\n");
		return 1;
	}

	void *handle_call(void *);

	sock=make_server_socket(atoi(av[1]));
	if(fd==-1){
		perror("cannot make socket");
		return 1;
	}
	void setup(pthread_attr_t *);
	setup(&attr);

	while(1){
		fd=accept(sock,NULL,NULL);
		server_requests++;
		fdptr=malloc(sizeof(int));
		*fdptr=fd;
		pthread_create(&worker,&attr,handle_call,(void *)fdptr);
	}

}

void setup(pthread_attr_t *attrp){
	pthread_attr_init(attrp);
	pthread_attr_setdetachstate(attrp,PTHREAD_CREATE_DETACHED);
	time(&server_stated);
	server_bytes_sent=0;
	server_requests=0;
}

void *handle_call(void *fdptr){
	int fd=*(int *)fdptr;
	free(fdptr);

	FILE *fpin;
	char request[BUFSIZ];

	fpin=fdopen(fd,"r");
	fgets(request,BUFSIZ,fpin);
	printf("got a call on %d: request=%s\n",fd,request);

	void skip_rest_of_header(FILE *);
	skip_rest_of_header(fpin);

	process_rq(request,fd);
	fclose(fpin);
}
void skip_rest_of_header(FILE *fp){
	char buf[BUFSIZ];
	while(fgets()!=NULL&&strcmp(buf,"\r\n")!=0);
}

void process_rq(char *rq,int fd){
	char cmd[BUFSIZ],arg[BUFSIZ];

	if(sscanf(rq,"%s%s",cmd,arg)!=2){
		return;
	}
	sanitize(arg);
	printf("sanitize version:%s\n",arg);
	void not_implemented(void);
	int built_in(char *,int);
	int not_exist(char *);
	void do_404(char *,int);
	int isadir(char *);
	void do_ls(char *,int);
	void do_cat(char *,int);
	if(strcmp(cmd,"GET")!=0){
		not_implemented();
	}else if(built_in(arg));
	else if(not_exist(arg)){
		do_404(arg,fd);
	}else if(isadir(arg)){
		do_ls(arg,fd);
	}else{
		do_cat(arg,fd);
	}
}

void sanitize(char *str){
	char *dst,*src;
	dst=src=str;

	while(*src){
		if(strncmp(src,"/../",4)==0){
			src+=3;
		}else if(strncmp(src,"//",2)==0){
			src++;
		}else{
			*dst++=*src++;
		}
	}

	*dst='\0';
	if(*str=='/'){
		strcpy(str,str+1);
	}
	if(*str='\0'||strcmp(str,"./")==0||strcmp(str,"./..")==0){
		strcpy(str,".");
	}
}

int http_reply(int fd,FILE **fpp,int code,char *msg,char *type,char *content){
	FILE *fp;
	int bytes=0;

	fp=fdopen(fd,"w");
	if(fp!=NULL){
		bytes=fprintf(fp,"HTTP/1.0 %d %s\r\n",code,msg);
		bytes+=fprintf(fp,"Content-type: %s\r\n\r\n",type);
		if(content){
			bytes+=fprintf(fp,"%s\r\n",content);
		}
	}
	fflush(fp);
	if(fpp){
		*fpp=fp;
	}else{
		fclose(fp);
	}
	return bytes;
}

int built_in(char *arg,int fd){
	FILE *fp;
	if(strcmp(arg,"status")!=0)return 0;
	http_reply(fd,&fp,200,"OK","test/plain",NULL);
	fprintf(fp,"Server started: %s\n",ctime(&server_stated));
	fprintf(fp,"Total requests: %d\n",server_requests);
	fprintf(fp,"Bytes sent out: %d\n",server_bytes_sent);
	fclose(fp);
	return 1;
}

void not_implemented(int fd){
	http_reply(fd,NULL,501,"Not Implemented","test/plain","That command is not implemented");
}

void do_404(char *item,int fd){
	http_reply(fd,NULL,404,"Not Found","test/plain","The item you seek is not here");
}

int isadir(char *f){
	struct stat info;
	return (stat(f,&info)!=-1&&S_ISDIR(info.st_mode));
}

int not_exist(char *f){
	struct stat info;
	return (stat(f,&info)==-1);
}

do_ls(char *dir,int fd){
	DIR *dirptr;
	struct dirent *direntp;
	int bytes=0;
	FILE *fp;

	bytes=http_reply(fd,&fp,200,"OK","text/plain",NULL);
	bytes+=fprintf(fp,"Listening of Directory %s\n",dir);

	dirptr=opendir(dir);
	if(dirptr!=NULL){
		while(direntp=readdir(dirptr)){
			bytes+=fprintf(fp,"%s\n",direntp->d_name);
		}
		closedir(dirptr);
	}
	fclose(fp);
	server_bytes_sent+=bytes;
	
}
char *file_type(char *f){
	char *cp;
	if((cp=strrchr(f,"."))!=NULL){
		return cp+1;
	}
	return "";
}

void do_cat(char *f,int fd){
	char *extension=file_type(f);
	char *type="text/plain";
	FILE *fpsock,*fpfile;
	int c,types=0;

	if(strcmp(extension,"html")==0){
		type="text/html";
	}else if(strcmp(extension,"gif")==0){
                type="image/gif";
        }else if(strcmp(extension,"jpg")==0){
                type="image/jpeg";
        }

	fpsock=fdopen(fd,"w");
	fpfile=fopen(f,"r");
	if(fpsock!=NULL&&fpfile!=NULL){
		bytes=http_reply(fd,&fpsock,200,"OK",type,NULL);
		while((c=getc(fpfile))!=EOF){
			putc(c,fpsock);
			bytes++;
		}
	fclose(fpsock);
	fclose(fpfile);
	}
	server_bytes_sent+=bytes;
}
// extension,type,fpsock,fpfile,
// strrchr,file_type
// DIR,dirptr,dirent,direntp,opendir,readdir,closedir
// status,test/plain,200,OK
// fd,fpp,code,msg,type,content,bytes,
// cmd,arg,sscanf,sanitize,not_implemented,built_in,not_exist,do_404,isadir,
// do_ls,do_cat
// fpin,request,free,skip_rest_of_header,process_rq,
// setup,pthread_attr_t,pthread_attr_init,pthread_attr_setdetachstate,
// PTHREAD_CREATE_DETACHED,
// time_t server_stated,server_bytes_sent,server_requests,fdptr,worker,attr
// handle_call,make_server_socket
