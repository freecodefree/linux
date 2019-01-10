#include "stdio.h"

#define oops(m,x) {perror(m);exit(x);}

int main(){
	int pid,todc[2],fromdc[2];

	if(pipe(todc)==-1||pipe(fromdc)==-1){
		oops("cannot get pipe",1);
	}

	if((pid=fork())==-1){
		oops("cannot fork",1);
	}

	if(pid==0){
		be_dc(todc,fromdc);
	}else{
		be_bc(todc,fromdc);
		wait(NULL);
	}
}

void be_dc(int in[],int out[]){
	if(dup2(in[0],0)==-1){
		oops("dc:cannot redirect stdin",2);
	}
	close(in[0]);
	close(in[1]);

	if(dup2(out[1],1)==-1){
		oops("dc:cannot redirect stdout",2);
	}
	close(out[0]);
	close(out[1]);

	execlp("dc","dc","-",NULL);
	oops("fail to execute dc",2);
	
}
// execlp
// pid,todc[2],fromdc,pipe,be_dc,be_bc,wait
