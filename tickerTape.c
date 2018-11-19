#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "curses.h"

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
	tape->x=atoi(av[1]);
	tape->y=atoi(av[2]);
	tape->len=atoi(av[3]);
	return 0;
}
