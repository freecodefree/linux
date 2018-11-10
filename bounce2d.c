#include "curses.h"
#include "signal.h"
#include "bounce.h"
#include "stdio.h"
#include "sys/time.h"
//#include "bounce1d.c"

struct ppball ball;
void setUp();
void wrapUp();
void moveBall(int);
int bounceOrLose(struct ppball *);
int setTicker(int);

int main(){
	int c;
	setUp();
	while((c=getchar())!='q'){
		switch (c){
			case 'f':ball.xTtm--;break;
			case 'g':ball.yTtm--;break;
			case 's':ball.xTtm++;break;
			case 'd':ball.yTtm++;break;
		}
	}
	wrapUp();
}
void setUp(){
	ball.xPos=X_INIT;
	ball.yPos=Y_INIT;
	ball.xTtg=ball.xTtm=X_TTM;
	ball.yTtg=ball.yTtm=Y_TTM;
	ball.xDir=1;
	ball.yDir=1;
	ball.symbol=DFL_SYMBOL;

	initscr();
	noecho();
	crmode();
//	signal(SIGINT,SIG_IGN);
	signal(SIGALRM,moveBall);
	setTicker(1000/TICKS_PER_SEC);	
//	mvaddch(ball.yPos,ball.xPos,ball.symbols);
//	move(LINS,COLS);

}

void wrapUp(){
	endwin();
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

void moveBall(int s){
	int yCur,xCur,moved;
	signal(SIGALRM,SIG_IGN);
	xCur=ball.xPos;
	yCur=ball.yPos;
	moved=0;

	if(ball.xTtm>0&&ball.xTtg--==1){
		ball.xTtg=ball.xTtm;
//		xCur=ball.xPos;
		ball.xPos+=ball.xDir;
		moved=1;
	}
	if(ball.yTtm>0&&ball.yTtg--==1){
                ball.yTtg=ball.yTtm;
//              yCur=ball.yPos;
                ball.yPos+=ball.yDir;
                moved=1;
        }
	if(moved){
		mvaddch(yCur,xCur,BLANK);
		mvaddch(ball.yPos,ball.xPos,ball.symbol);
		move(0,0);
//		move(LINES-1,COLS-1);
		refresh();
		bounceOrLose(&ball);
	}
	signal(SIGALRM,moveBall);

}

int bounceOrLose(struct ppball *b){
	int rtval=0;
	if(b->yPos>BOT_ROW){
		b->yDir=-1;
		rtval=1;
	}else if(b->yPos<TOP_ROW){\
		b->yDir=1;
		rtval=1;
	}
	if(b->xPos>RIGHT_EDGE){
                b->xDir=-1;
                rtval=1;
        }else if(b->xPos<LEFT_EDGE){\
                b->xDir=1;
                rtval=1;
        }
	
	return rtval;	
}
// yCur,xCur,moved,
// initscr,noecho,crmode,SIG_IGN,mvaddch,refresh,setTicker
// setUp,wrapUp
