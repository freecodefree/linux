#include "curses.h"
#include "signal.h"
#include "bounce.h"
#include "stdio.h"

struct ppball ball;
void setup();
void wrapUp();
void moveBall(int);
int bounceOrLose(struct ppball *);

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
	ball.symbols=DFL_SYMBOL;

	initscr();
	noecho();
	crmode();
//	signal(SIGINT,SIG_IGN);
	signal(SIGALRM,moveBall);
	setTicker(TICKS_PER_SEC/50);	
//	mvaddch(ball.yPos,ball.xPos,ball.symbols);
//	move(LINS,COLS);

}

void moveBall(int s){
	int yCur,xCur,moved;
	signal(SIGALRM,SIG_IGN);
	moved=0;

	if(ball.xTtm>0&&ball.xTtg--==1){
		ball.xTtg=ball.xTtm;
		xCur=ball.xPos;
		ball.xPos+=ball.xDir;
		moved=1;
	}
	if(ball.yTtm>0&&ball.yTtg--==1){
                ball.yTtg=ball.yTtm;
                yCur=ball.yPos;
                ball.yPos+=ball.yDir;
                moved=1;
        }
	if(moved){
		mvaddch(yCur,xCur,BLANK);
		mvaddch(ball.yPos,ball.xPos,ball.symbols);
		move(LINES,COLS);
		refresh();
		bounceOrLose(&ball);
	}
	signal(SIGALRM,moveBall);

}
// yCur,xCur,moved,
// initscr,noecho,crmode,SIG_IGN,mvaddch,refresh,setTicker
// setUp,wrapUp
