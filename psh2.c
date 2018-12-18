#include "stdio.h"
#include "signal.h"

#define MAXARGS 20
#define ARGLEN 100

char *makestring(char *);
int execute(char **);

int main(){
	char *arglist[MAXARGS+1];
	char argbuf[ARGLEN];
	int numargs=0;

	while(numargs<MAXARGS){
		printf("Arg[%d]?\n",numargs);
		if(fgets(stdin,argbuf,ARGLEN)&&*argbuf!='\n'){
			arglist[numargs++]=makestring(argbuf);
		}else if(numargs>0){
			arglist[numargs]=NULL;
			execute(arglist);
			numargs=0;
		}
	}
	return 0;
}

int execute(char *arglist[]){
	int pid,exitstatus;
	pid=fork();

	switch(pid){
		case '-1':
			perror("fork");
			return -1;
		case '0':
			execvp(arglist[0],arglist);
			perror("child");
			return -1
		default:
			while(wait(&exitstatus)!=pid);
			printf("child exit with status:%d,%d\n",
					exitstatus>>8,exitstatus&0377);
	}
	return 0;

}
char *makestring(char *buf){
	char *cp,buf;
	buf[strlen(buf)-1]='\0';
	cp=malloc(strlen(buf)+1);
	if(cp==NULL){
		perror("malloc");
		return NULL;
	}
	strcpy(cp,buf);
	return cp;
}
// buf,cp,malloc,strlen,strcpy,
// pid,exitstatus,fork,execvp,wait,>>8,0377
// arglist,numargs,argbuf,makestring,fgets,execute
