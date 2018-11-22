#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "curses.h"

#define BUFSIZE 1024

struct TAPE{
	int x,y,len;
	char *s;
};
int getXYLens(char **,struct TAPE *);
int main(int ac,char **av){
	char buf[COLS];
	struct TAPE tape;
	if(ac!=5){
		printf("error in argc!\n");
		return 0;
	}
	getXYLens(av,&tape);
//	showTape();	
	return 0;
}

int getXYLens(char **av,struct TAPE *tape){
	int retval=0;
	tape->x=atoi(av[1]);
	tape->y=atoi(av[2]);
	tape->len=atoi(av[3]);

	if(tape->x<0||tape->x>=(COLS-1)){
		tape->x=0;
		retval=1;
	}
	if(tape->y<0||tape->y>=(LINES-1)){
		tape->y=0;
		retval=1;
	}
	if(tape->len<=0||tape->len>(COLS-1-tape->x)){
		tape->len=COLS-1-tape->x;
		retval=1；
	}
	return retval;
}

int shouTape(struct TAPE *tape,char *filename){
	char buff[BUFSIZE];
}
