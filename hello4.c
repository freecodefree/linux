#include "stdio.h"
#include "curses.h"

#define ROW 10
#define LINE 10
#define SPEED 1
#define WORD "hello world"
#define BLANK "           "
int main(){
	int x,y,dir=SPEED;
	initscr();
	clear();
	x=y=5;
	while(1){
		if(x>ROW||x<0){
	//		x=5;
			dir*=-1;
		}
		else if(y>LINE||y<0){
          //              y=5;
                        dir*=-1;
                }
		move(x,y);
		addstr(WORD);
//		refresh();
		move(0,0);
		refresh();
		sleep(1);
		move(x,y);
		addstr(BLANK);
//		move(0,0);
//		refresh();
//		sleep(1);
		x+=dir;
		y+=dir;
	}
	return 0;
}
