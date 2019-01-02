#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "signal.h"
#include "sys/wait.h"
#include "smsh.h"

#define DFL_PROMPT "> "
void setup();
int main(){
	char *cmdline,*prompt,**arglist;
	int result=0,process(char **);
	void setup();

	prompt=DFL_PROMPT;
	setup();

	while((cmdline=nextCmd(prompt,stdin))!=NULL){
		if((arglist=splitline(cmdline))!=NULL){
			result=process(arglist);
			freelist(arglist);
		}
		free(cmdline);
	}
	return 0;
}
// cmdline,prompt,arglist,result,process,setup,nextCmd,splitline,freelist,free
