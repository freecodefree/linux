#include "stdio.h"
#include "sys/time.h"
#include "curses.h"
#include "string.h"
#include "signal.h"

#define MESSAGE "hello"
#define BLANK   "     "

int row,col,dir,speed;
int setTicker(int);
void moveMsg(int);
int main(){
	int delay,ndelay;
	int c;

	initscr();
	crmode();
	noecho();
	clear();
	signal(SIGALRM,moveMsg);

	row=10;
	col=2;
	dir=1;
	speed=1;
	move(row,col);
	addstr(MESSAGE);
	refresh();
	set_ticker(delay);
	while(1){
		c=getchar();
		if(c=='q'){
			break;
		}
		if(c==' '){
			dir*=-1;
		}
		if(c=='s'){
			speed--;
			if(speed<0){
				speed=0;
			}
		}
	        if(c=='f'){
                        speed++;
                        if(speed>6){
                                speed=6;
                        }
                }
	}
	endwin();
	return 0;
}

void moveMsg(int s){
	move(row,col);
	addstr(BLANK);
	col+=speed*dir;
	if(col>COLS-strlen(BLANK)||col<1){
		dir*=-1;
	}
	move(row,col);
	addstr(MESSAGE);
	refresh();
}

int setTicker(int nMSecs){
	struct iti
}
// row,col,dir,delay,ndelay,moveMsg,initscr,crmode,noecho,clear,move,addstr
// signal,SIGALRM,set_ticker,endwin
