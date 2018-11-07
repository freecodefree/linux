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
	delay=1000;
	move(row,col);
	addstr(MESSAGE);
	refresh();
	setTicker(delay);
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
			ndelay=delay*speed;
			setTicker(ndelay);
		}
	        if(c=='f'){
                        speed++;
                        if(speed>6){
                                speed=6;
                        }
			ndelay=delay*speed;
			setTicker(ndelay);
                }
	}
	endwin();
	return 0;
}

void moveMsg(int s){
	move(row,col);
	addstr(BLANK);
//	col+=speed*dir;
	col+=dir;
	if(col>(COLS-strlen(BLANK))||col<1){
		dir*=-1;
	}
	move(row,col);
	addstr(MESSAGE);
	refresh();
}

int setTicker(int nMSecs){
	struct itimerval newTimeset;
	long nSec,nUSecs;

	nSec=nMSecs/1000;
	nUSecs=(nMSecs%1000)*1000L;

	newTimeset.it_interval.tv_sec=nSec;
	newTimeset.it_interval.tv_usec=nUSecs;
	newTimeset.it_value.tv_sec=nSec;
	newTimeset.it_value.tv_usec=nUSecs;

	return setitimer(ITIMER_REAL,&newTimeset,NULL);
}
// nMSecs,itimerval,newTimeset,nSec,nUSecs,it_interval,tv_sec,tv_usec,it_value
// ITIMER_REAL,setitimer
// row,col,dir,delay,ndelay,moveMsg,initscr,crmode,noecho,clear,move,addstr
// signal,SIGALRM,set_ticker,endwin
