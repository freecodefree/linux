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
// arglist,numargs,argbuf,makestring,fgets,execute
