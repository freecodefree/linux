#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "signal.h"
#include "smsh.h"

#define DFL_PROMPT "> "
int setup(void);
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
// cmdline,prompt,arglist,result,setup,next_cmd,splitline,execute,freelist
