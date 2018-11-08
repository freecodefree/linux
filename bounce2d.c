#include "curses.h"
#include "signal.h"
#include "bounce.h"
#include "stdio.h"

struct ppball ball;
void setup();
void wrapUp();

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
// setUp,wrapUp
