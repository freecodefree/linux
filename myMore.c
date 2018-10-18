#include "stdio.h"
#include "signal.h"
#include "termios.h"
#include "fcntl.h"
#include "stdlib.h"
#define PAGELINE 20
#define LINELEN 20
int doMore(FILE *);
int seeMore();
int ttyMode(int);
int setCrNoechoMode();
void ctrlCQHandler(int);

int ttyMode(int n){
	static struct termios originalMode;
	static int originalFlags;
	static int stored=0;

	if(n==0){
		tcgetattr(0,&originalMode);
		originalFlags=fcntl(0,F_GETFL);
		stored=1;
		return 0;
	}else if(stored){
		tcsetattr(0,TCSANOW,&originalMode);
		fcntl(0,F_SETFL,originalFlags);
		return 0;
	}
	return 1;
}

int setCrNoechoMode(){
	struct termios ttyState;
	tcgetattr(0,&ttyState);
	ttyState.c_lflag&=~ICANON;
	ttyState.c_lflag&=~ECHO;
	ttyState.c_cc[VMIN]=1;
	tcsetattr(0,TCSANOW,&ttyState);
	return 0;
}
// termios,ttyState,tcgetattr,c_lflag,ICANON,ECHO,c_cc,VMIN,TCSANOW
// tcsetattr,originalMode,originalFlags,stored,fcntl,F_SETFL,F_GETFL,
#if 1

int main(int ac,char **av){
	if(ac==1){
		doMore(stdin);
		return 0;
//		printf("usage:more [filename]");
//		return 1;
	}
	FILE *fp;
	fp=fopen(*++av,"r");
	if(fp==NULL){
		printf("error in main:can't open file %s\n",*av);
		return 2;
	}
        ttyMode(0);
        signal(SIGINT,ctrlCQHandler);
        signal(SIGQUIT,ctrlCQHandler);
        setCrNoechoMode();
	doMore(fp);
	ttyMode(1);
	fclose(fp);
	return 0;
}

#endif

int doMore(FILE *fp){
	if(fp==NULL){
		printf("error in doMore:nothing to do!\n");
		return 1;
	}
	char line[LINELEN];
	int numOfLine=0;
	while(fgets(line,LINELEN,fp)){
		if(fputs(line,stdout)==EOF){
                        return 0;
                }
                numOfLine++;

		int repeat;
		if(numOfLine>=PAGELINE){
			repeat=seeMore();
			printf("\r");
			for(int i=0;i<LINELEN-1;i++){
				putchar(' ');
			}
			putchar('\r');
			printf("\033[1A");
		
		if(repeat){
			numOfLine-=repeat;
		}else{
			printf("\r");
                        for(int i=0;i<LINELEN-1;i++){
                                putchar(' ');
                        }
                        putchar('\r');
			return 0;
		}
		}
//		if(fputs(line,stdout)==EOF){
//			return 0;
//		}
//		numOfLine++;
	}
//	putchar('v');
	return 0;
}

void ctrlCQHandler(int signum){
	printf("\r");
                        for(int i=0;i<LINELEN-1;i++){
                                putchar(' ');
                        }
                        putchar('\r');
	ttyMode(1);

	exit(1);
}

int seeMore(){
	FILE *cmd;
	cmd=fopen("/dev/tty","r");
	if(cmd==NULL){
		return PAGELINE;
	}
	printf("\033[7m[q]:quit;[SPACE]:next page;[ENTER]:next line\033[m");
	int c;
//	ttyMode(0);
//	signal(SIGINT,ctrlCQHandler);
//	signal(SIGQUIT,ctrlCQHandler);
//	setCrNoechoMode();
	while((c=getchar())!=EOF){
	if(c=='q'){
		return 0;
	}
	if(c==' '){
//		putchar('\n');
		return PAGELINE;
	}
	if(c=='\n'){
		return 1;
	}
	}
	return PAGELINE;
}
