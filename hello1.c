#include "stdio.h"
#include "curses.h"

int main(){
	initscr();
	clear();
	move(10,10);
	addstr("hello worle");
	move(LINES-1,1);
	refresh();
	endwin();
}
// initscr,clear, move,addstr,refresh,endwin
