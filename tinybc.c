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
// pid,todc[2],fromdc,pipe,be_dc,be_bc,wait
