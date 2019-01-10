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

void be_bc(int todc[],int fromdc[]){
	int num1,num2;
	char operation[BUFSIZ],message[BUFSIZ];
	FILE *fpout,*fpin;

	close(todc[0]);
	close(fromdc[1]);
	
	fpout=fdopen(todc[1],"w");
	fpin=fromdc(fromdc[0],"r");
	if(fpout==NULL||fpin==NULL){
		fatal("error converint pipes to streams");
	}
	
	while(1){
		printf("tinybc:");
		if(fgets(message,BUFSIZ,stdin)==NULL)break;

		if(sscanf(message,"%d%[+-*/^]%d",&num1,operation,&num2)!=3){
			printf("syntax error\n");
			continue;
		}

		if(fprintf(fpout,"%d\n%d\n%c\np\n",num1,num2,*operation)==EOF){
			fatal("error writing",3);
		}
		fflush(fpout);

		if(fgets(message,BUFSIZ,fpin)==NULL)break;
		printf("%d %c %d = %s\n",num1,*operation,num2,message);
	}
	fclose(fpin);
	fclose(fpout);
}
// 
// execlp,num12,operation,BUFSIZ,message,fgets,fpout,fpin,fdopen
// pid,todc[2],fromdc,pipe,be_dc,be_bc,wait
