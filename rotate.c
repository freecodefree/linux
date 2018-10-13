#include "stdio.h"
#include "termios.h"
#include "fcntl.h"
#include "signal.h"
#include "stdlib.h"
#define BEEP putchar('\a')
// ctrlCQHandler,ttyMode,signal,setCrNoechoMode,rotate,SIGINT,SIGQUIT,
// how,tcgetattr,termios,originalMode,stored,originalFlags,F_GETFL,
// fcntl,tcsetattr,TCSANOW,
// ttyState,c_lflag,ICANON,ECHO,VMIN,

int setCrNoechoMode(){
	struct termios ttyState;

	tcgetattr(0,&ttyState);
	ttyState.c_lflag&=~ICANON;
	ttyState.c_lflag&=~ECHO;
	ttyState.c_cc[VMIN]=1;
	tcsetattr(0,TCSANOW,&ttyState);

	return 0;	
}

int ttyMode(int);
void ctrlCQHandler(int n){
	ttyMode(1);
	BEEP;
	exit(1);
}

int ttyMode(int how){
	static struct termios originalMode;
	static int originalFlags;
	static int stored=0;

	if(how==0){
		tcgetattr(0,&originalMode);
		originalFlags=fcntl(0,F_GETFL);
		stored=1;
	}else if(stored){
		tcsetattr(0,TCSANOW,&originalMode);
		fcntl(0,F_SETFL,originalFlags);
	}
	return 0;
}

int main(){
	ttyMode(0);
	signal(SIGINT,ctrlCQHandler);
	signal(SIGQUIT,ctrlCQHandler);
	sleep(3);
	BEEP;
	fflush(stdout);
//	BEEP;
	setCrNoechoMode();
	sleep(3);
//	rotate();
	ttyMode(1);
	return 0;
}
