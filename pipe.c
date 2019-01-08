#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"

#define oops(m,x) {perror(m);exit(x);}

int main(int ac,char **av){
	int thepipe[2],newfd,pid;

	if(ac!=3){
		exit(7);
	}
	
	if(pipe(thepipe)==-1){
		fprintf(stderr,"cannot get a pipe!\n");
		exit(1);
	}

	if((pid=fork())==-1){
		fprintf(stderr,"cannot fork!\n");
		exit(2);
	}

	if(pid>0){
		close(thepipe[1]);
		if(dup2(thepipe[0],0)==-1){
			oops("cannot redirect stdin",3);
		}
		close(thepipe[0]);
		execlp(av[2],av[2],NULL);
		oops("cannot execute av2",4);
	}else{
		close(thepipe[0]);
		if(dup2(thepipe[1],1)==-1){
			oops("cannot redirect stdout",5);
		}
		close(thepipe[0]);
		execlp(av[1],av[1],NULL);
		oops("cannot execute av1",6);
	}

}
// thepipe,newfd,pid,pipe,close,dup2,execlp,oops
