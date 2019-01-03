#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "signal.h"
#include "sys/wait.h"
#include "smsh.h"
#include "string.h"

enum states {NEUTRAL,WANT_THEN,THEN_BLOCK};
enum results {SUCCESS,FAIL};

static int if_state=NEUTRAL;
static int if_result=SUCCESS;
static int last_stat=0;

int syn_err(char *);



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

int ok_to_execute(){
	int rv=1;
	if(if_state==WANT_THEN){
		syn_err("expect then block\n");
		rv=0;
	}else if(if_state==THEN_BLOCK&&if_result==SUCCESS){
		rv=1;
	}else(if_state==THEN_BLOCK&&if_result==FAIL){
		rv=0;
	}
	return rv;
}

int is_control_command(char *s){
	return (strcmp(s,"if")==0||strcmp(s,"then")==0||strcmp(s,"fi")==0);
}

int do_control_command(char **args){
	char *cmd=*args;
	int rv=-1;

	if(strcmp(cmd,"if")==0){
		if(if_state!=NEUTRAL){
			rv=syn_err("unexpected if");
		}else{
			last_stat=process(args+1);
			if_result=(last_stat==0?SUCCESS:FAIL);
			if_state=WANT_THEN;
			rv=0;
		}
	}else if(strcmp(cmd,"then")==0){
		if(if_state!=WANT_THEN){
			rv=syn_err("unexpected then");
		}else{
			if_state=THEN_BLOCK;
			rv=0;
		}
	}else if(strcmp(cmd,"fi")==0){
		if(if_state!=THEN_BLOCK){
			rv=syn_err("unexpected fi");
		}else{
			if_state=NEUTRAL;
			rv=0;
		}
	}else{
		fatal("internal error processing:",cmd,2);
	}

	return rv;
}

int syn_err(char *msg){
	if_state=NEUTRAL;
	fprintf(stderr,"syntax error:%s\n",msg);
	return -1;
}
// cmd,if_state,NEUTRAL,syn_err,last_stat,if_result,WANT_THEN,
// cmdline,prompt,arglist,result,process,setup,nextCmd,splitline,freelist,free
