#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "signal.h"
#include "sys/wait.h"
#include "smsh.h"
#include "string.h"
#include "ctype.h"


enum states {NEUTRAL,WANT_THEN,THEN_BLOCK};
enum results {SUCCESS,FAIL};

static int if_state=NEUTRAL;
static int if_result=SUCCESS;
static int last_stat=0;

int syn_err(char *);


#define MAXVARS 200
#define DFL_PROMPT "> "
struct var{
	char *str;
	int global;
};
static struct var tab[MAXVARS];
static struct var *find_item(char *,int);
static char *new_string(char *,char *);
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
	extern char **environ;
	VLenviron2table(environ);
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
		if(!builtin_command(args,&rv))
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

int builtin_command(char **args,int *resultp){
	int rv=0;

	if(strcmp(*args,"set")==0){
		VLlist();
		*resultp=0;
		rv=1;
	}else if(strchr(args[0],'=')!=NULL){
		*resultp=assign(args[0]);
		if(*resultp!=-1)
			rv=1;		
	}else if(strcmp(*args,"export")==0){
		if(args[1]!=NULL&&okname(args[1])){
			*resultp=VLexport(args[1]);
		}else{
			*resultp=1;
		}
		rv=1;
	}
	return rv;
}
int assign(char *str){
	int rv=0;
	char *cp;
	cp=strchr(str,'=');
	*cp='\0';
	rv=(okname(str)?VLstore(str,cp+1):-1);
	cp='=';
	return rv;
}
int okname(char *str){
	char *cp;
	for(cp=str;*cp;cp++){
		if((isdigit(cp)&&(cp==str))||(!isalnum(cp)&&cp!='_'))return 0;
	}
	return (cp!=str);
}

int execute(char **args){
	int pid,child=-1;
	if(args[0]==NULL){
		return 0;
	}

	if((pid=fork())==-1){
		perror("fork");
	}else if(pid==0){
		environ=VLtable2environ();
		signal(SIGINT,SIG_DFL);
		signal(SIGQUIT,SIG_DFL);
		execvp(args[0],args);
		perror("cannot execute command");
		exit(1);
	}else{
		if(wait(&child)==-1){
			perror("wait");
		}
//		return child;
	}
	return child;

}
int VLstore(char *name,int val){
	int rv=-1;
	char *s;
	struct var *itemp;

	if((itemp=find_item(name,1))!=NULL&&(s=new_string(name,val))!=NULL){
		if(itemp->str!=NULL){
			free(itemp->str);
		}
		itemp->str=s;
		rv=0;
	}
	return rv;
}

char *new_string(char *name,char *val){
	char *rv;
	rv=malloc(strlen(name)+2+strlen(val));
	if(rv!=NULL){
		sprintf(rv,"%s=%s",name,val);
	}
	return rv;
}
char *VLlookup(char *name){
	struct var *itemp;
	if((itemp=find_item(name,0))!=NULL){
		return (itemp->str+1+strlen(name));
	}
	return "";
}

int VLexport(char *name){
	int rv=1;
	struct var *itemp;
	if((itemp=find_item(name,0))!=NULL){
		itemp->global=1;
		rv=0;
	}else if(VLstore(name,"")==1){
		rv=VLexport(name);
	}
	return rv;
}

static struct var *find_item(char *name,int first_blank){
	char *s;
	int len=strlen(name);
	int i;	

	for(i=0;i<MAXVARS&&(s=tab[i].str)!=NULL;i++){
		if(strncmp(s,name,len)==0&&s[len]=='='){
			return &tab[i];
		}
	}
	if(i<MAXVARS&&first_blank){
		return &tab[i];
	}
	return NULL;
}
void VLlist(){
	for(int i=0;i<MAXVARS&&tab[i].str!=NULL;i++){
		if(tab[i].global==1){
			printf("* %s\n",tab[i].str);
		}else{
			printf("  %s\n",tab[i].str);
		}
	}
}

int VLenviron2table(char *env[]){
	int i;
	for(i=0;env[i]!=NULL;i++){
		if(i==MAXVARS){
			return 0;
		}
		char *newstring;
		newstring=malloc(strlen(env[i])+1);
		if(newstring==NULL){
			return 0;
		}
		strcpy(newstring,env[i]);
		tab[i].str=newstring;
		tab[i].global=1;
	}
	while(i<MAXVARS){
		tab[i].str=NULL;
		tab[i++].global=0;
	}
	return 1;
}

char **VLtable2environ(){
	int n=0,j=0;
	char **envtab;
	for(int i=0;tab[i].str!=NULL;i++){
		if(tab[i].global==1){
			n++;
		}
	}
	envtab=(char **)malloc((1+n)*sizeof(char *));
	if(envtab==NULL)return NULL;
	for(int i=0;i<MAXVARS&&tab[i].str!=NULL;i++){
		if(tab[i].global==1){
			envtab[j++]=tab[i].str;
		}
	}
	envtab[j]=NULL;
	return envtab;
}
// VLtable2environ,envtab,
// VLlist,VLenviron2table,env,newstring,
// first_blank,len,MAXVARS,tab,str,strncmp,
// VLlookup,find_item,var,VLstore,global
// VLstore,name,val,itemp,
// VLtable2environ,execvp 
// str,cp,strchr,VLstore,isalnum
// VLlist,resultp,strchr,assign,VLexport,okname
// cmd,if_state,NEUTRAL,syn_err,last_stat,if_result,WANT_THEN,
// cmdline,prompt,arglist,result,process,setup,nextCmd,splitline,freelist,free
