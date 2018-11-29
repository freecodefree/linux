#include "stdio.h"
#include "signal.h"
#include "string.h"

#define MAXARGS 20
#define ARGLEN 100

int execute(char **);
char *makestring(char *);

int main(){
	char *arglist[MAXARGS+1];
	int numargs=0;
	char argbuf[ARGLEN];

	while(numargs<MAXARGS){
		printf("Arg[%d]?",numargs);
		if(fgets(argbuf,ARGLEN,stdin)&&*argbuf!='\n'){
			arglist[numargs++]=makestring(argbuf);
		}else{
			if(numargs>0){
				arglist[numargs]=NULL;
				execute(arglist);
				numargs=0;
			}
		}
	}
	return 0;

}
int execute(char *arglist[]){
	execvp(arglist[0],arglist);
	perror("execvp:");
	return 1;
}

char *makestring(char *buf){
	char *cp;
	buf[strlen(buf)-1]='\0';
	if((cp=malloc(strlen(buf)+1))==NULL){
		printf("error:malloc\n");
		exit (1);
	}
	strcpy(cp,buf);
	return cp;
}


// execvp,cp,malloc,buf,strcpy
// arglist,numargs,argbuf,makestring,numargs,execute
