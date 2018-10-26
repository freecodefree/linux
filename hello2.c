#include "stdio.h"
#include "curses.h"

int main(){
	initscr();
	for(int i=0;i<LINES;i++){
		clear();
		move(i,i+1);
		if(i%2){
			standout();
		}
		addstr("hello world");
		standend();
		refresh();
		getch();
	}
//	getch();
	endwin();
}
// initscr,clear,LINES,move,standout,addstr,standend,refresh,getch,endwin
