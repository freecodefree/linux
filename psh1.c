#include "stdio.h"
#include "signal.h"
#include "string.h"

#define MAXARGS 20
#define ARGLEN 100

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
// arglist,numargs,argbuf,makestring,numargs,execute
