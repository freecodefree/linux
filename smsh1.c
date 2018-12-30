#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "signal.h"
#include "sys/wait.h"
#include "smsh.h"

#define DFL_PROMPT "> "
int setup(void);
int execute(char *argv[]);
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

// pid,childInfo,fork,SIG_DFL,execvp,wait
// signal,SIG_IGN,SIGINT,SIGQUIT
// cmdline,prompt,arglist,result,setup,next_cmd,splitline,execute,freelist
