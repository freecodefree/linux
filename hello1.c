#include "stdio.h"
#include "curses.h"
//#include "stdlib.h"
#include "unistd.h"

int main(int ac,char **av){
	char *s;
	initscr();
	clear();
	move(10,10);
	if(ac==2){
		s=*++av;
	}else{
		s="hello worle";
	}
	while(1){
	        move(10,10);
		addstr(s);
		move(0,0);
		refresh();
		sleep(1);
		clear();
		refresh();
		sleep(1);
	}
//	move(LINES-1,1);
//	refresh();
//	getch();
	endwin();
	
}
// initscr,clear, move,addstr,refresh,endwin
