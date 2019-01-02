#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "signal.h"
#include "sys/wait.h"
#include "smsh.h"
#include "string.h"

#define DFL_PROMPT "> "
int setup(void);
int execute(char *argv[]);
char *next_cmd(char *,FILE *);
char **splitline(char *);

int main(int ac,char **av){
	char *cmdline,*prompt,**arglist;
	int result;
	prompt=DFL_PROMPT;
	setup();

	while((cmdline=next_cmd(prompt,stdin))!=NULL){
		if((arglist=splitline(cmdline))!=NULL){
			execute(arglist);
			freelist(arglist);
		}
		free(cmdline);
		
	}
	return 0;
}

int setup(){
	signal(SIGINT,SIG_IGN);
	signal(SIGQUIT,SIG_IGN);
}

int execute(char *argv[]){
	int pid,childInfo;
	childInfo=-1;

	if(argv[0]==NULL){
		return 0;
	}

	if((pid=fork())==-1){
		perror("fork");
		return childInfo;
	}

	if(pid==0){
		signal(SIGINT,SIG_DFL);
		signal(SIGQUIT,SIG_DFL);
		execvp(argv[0],argv);
		perror("cannot execute command");
		return 0;
	}else{
		if(wait(&childInfo)==-1){
			perror("wait");
		}
		return childInfo;
	}
}

char *next_cmd(char *prompt,FILE *fp){
	char *buf;
	int bufspace=0,pos=0;
	char c;

	printf("%s",prompt);

	while((c=getc(fp))!=EOF){
		if(pos+1>=bufspace){
			if(bufspace==0){
				buf=emalloc(BUFSIZ);
			}else{
				buf=erealloc(buf,bufspace+BUFSIZ);
				bufspace+=BUFSIZ;
			}
		}

		if(c=='\n')break;
		buf[pos++]=c;
	}

	if(pos=0&&c==EOF){
		return NULL;
	}else{
		buf[pos]='\0';
	}
	return buf;
}

char **splitline(char *line){
	char *cp,*start,*newstr(),**args;
	int bufspace=0,spots=0,argnum=0,len=0;

	if(line==NULL){
		return NULL;
	}
	cp=line;

	while(*cp!='\0'){
		if(is_delim(*cp++))continue;
		if(argnum+1>=spots){
			if(spots==0){
				args=emalloc(BUFSIZ);
				bufspace=BUFSIZ;
				spots=bufspace/sizeof(char *);
			}
			args=erealloc(args,BUFSIZ+bufspace);
			bufspace+=BUFSIZ;
			spots+=(BUFSIZ/sizeof(char *));
		}
		start=cp;
		len=1;
		while(*cp++!='\0'&&!(is_delim(*cp))){
			len++;
		}
		*args[argnum++]=newstr(start,len);
	}
	*args[argnum]=NULL;
	return args;
}
// line,newstr,args,spots,bufspace,argnum,cp,start,len,emalloc,sizeof
// is_delim,erealloc,start
// nextCmd,buf,bufspace,pos,prompt,FILE,getc,BUFSIZ,emalloc,erealloc
// pid,childInfo,fork,SIG_DFL,execvp,wait
// signal,SIG_IGN,SIGINT,SIGQUIT
// cmdline,prompt,arglist,result,setup,next_cmd,splitline,execute,freelist
