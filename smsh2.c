#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "signal.h"
#include "sys/wait.h"
#include "smsh.h"

#define DFL_PROMPT "> "
void setup();
void fatal();
int is_control_command(char *);
int do_control_command(char **);
int ok_to_execute();
int process(char **);

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

void setup(){
	signal(SIGINT,SIG_IGN);
	signal(SIGQUIT,SIG_IGN);
}

void fatal(char *s1,char *s2,int n){
	fprintf(stderr,"Error:%s,%s\n",s1,s2);
	exit(n);
}

int process(char **args){
	int rv=0;
	if(args[0]==NULL){
		return rv;
	}else if(is_control_command(args[0])){
		rv=do_control_command(args);
	}else if(ok_to_execute()){
		rv=execute(args);
	}
	return rv;
}
// 
// cmdline,prompt,arglist,result,process,setup,nextCmd,splitline,freelist,free
