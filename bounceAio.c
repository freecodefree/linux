#include "curses.h"
#include "signal.h"
#include "fcntl.h"
#include "bounce.h"
#include "unistd.h"
#include "stdio.h"
#include "sys/time.h"
#include "aio.h"
//#include "bounce1d.c"

struct ppball ball;
void setUp();
void wrapUp();
void moveBall(int);
int bounceOrLose(struct ppball *);
int setTicker(int);
void onInput(int);
int done=0;
//void enableKDBSignals();
void setupAioBuffer();

struct aiocb kbcbuf;

int main(){
	int c;
	setUp();
	signal(SIGIO,onInput);
	setupAioBuffer();
	aio_read(&kbcbuf);
//	enableKDBSignals();
	while(!done)pause();
	while(!done){
		c=getchar();
		switch (c){
			case 'f':ball.xTtm--;break;
			case 'g':ball.yTtm--;break;
			case 's':ball.xTtm++;break;
			case 'd':ball.yTtm++;break;
		}
	}
	wrapUp();
}

// aio_error,aio_return,
void setupAioBuffer(){
	static char input[1];

	kbcbuf.aio_fildes=0;
	kbcbuf.aio_buf=input;
	kbcbuf.aio_nbytes=1;
	kbcbuf.aio_offset=0;

	kbcbuf.aio_sigevent.sigev_notify=SIGEV_SIGNAL;
	kbcbuf.aio_sigevent.sigev_signo=SIGIO;
}
// aiocb,kbcbuf,setupAioBuffer,aioRead,input,aio_fildes,aio_buf
// aio_nbytes,aio_offset,aio_sigevent,sigev_notify,sigev_signo
// SIGEV_SIGNAL,SIGIO
void onInput(int s){
	if(aio_error(&kbcbuf)!=0){
		perror("aio read");
	}else if(aio_return(&kbcbuf)==1){
		char *cp=(char *)kbcbuf.aio_buf;
		char c=*cp;
		if(c=='q'||c==EOF){
			done =1;
		}
	}
	aio_read(&kbcbuf);
}

void enableKDBSignals(){
	int fdFlags;
	fcntl(0,F_SETOWN,getpid());
	fdFlags=fcntl(0,F_GETFL);
//	fcntl(0,F_SETFL,(fdFlags|O_ASYNC));
}
// fdFlags,F_SETOWN,getpid,F_GETFL,F_SETFL,O_ASYNC
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
