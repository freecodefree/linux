#define BLANK ' '
#define DFL_SYMBOL 'O'
#define TOP_ROW 5
#define BOT_ROW 20
#define LEFT_EDGE 10
#define RIGHT_EDGE 70
#define X_INIT 10
#define Y_INIT 10
#define TICKS_PER_SEC 50

#define X_TTM 5
#define Y_TTM 8

struct ppball{
	int yPos,xPos,
	    yTtm,xTtm,
	    yTtg,xTtg,
	    yDir,xDir;
	char symbol;
};
