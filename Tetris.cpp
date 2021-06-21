/*
          HiLCoE School of Computer Science & Technology

          Programming Assignment on TETRIS

          Done By: Elroe Tadele, Fitsum Mekonen, Ezana Haileselassie, Kaleab Cherenet

          DRB1802 Section B
          April 04, 2019
*/

#include <string>
#include <ctype.h>
#include <graphics.h>
#include <time.h>
#include <ctime>
#include <stdio.h> //for sprintf used for displaying score and level in the main function
#include <fstream>

using namespace std;

const int NB_BLOCKS = 5;
const int BAR_BLOCK = 1;
const int LR_BLOCK   = 2;
const int T_BLOCK   = 3;
const int BOX_BLOCK  = 4;
const int L_BLOCK = 5;
const int HORIZONTAL = 1;
const int VERTICAL = 2;
const int NORTH = 1;
const int WEST = 2;
const int SOUTH = 3;
const int EAST = 4;
const int RIGHT = 1;
const int LEFT = 2;
const int DOWN  = 3;

const int BOARD_COLOR   = BLACK;
const int BKGRD_COLOR   = BOARD_COLOR;
const int FILLING_COLOR = GREEN;
const int FRGRD_COLOR   = 1;
const int GRID_COLOR    = BKGRD_COLOR; //YELLOW;
const int BORDER_COLOR  = GRID_COLOR;

const int LEVEL1_SPEED 	= 800;
const int LEVEL2_SPEED  = 500;
const int LEVEL3_SPEED  = 200;

const int BOARDWIDTH 	= 290;//180;
const int BOARDHEIGHT	= 505;//240;
const int NB_ROWS  	= 20;//15;
const int NB_COLS  	= 10;
const int CELL_SIZE	= BOARDHEIGHT/(NB_ROWS+1);
const int LEFTCORNER	=230;
const int TOPCORNER	= 50; //120;
const int INFOAREAWIDTH	= 710;
const int INFOAREAHEIGHT= 105;
const int INFOAREALEFT	= 20;
const int INFOAREATOP	= 550;

const int NEXTBLOCK_LEFTCORNER = LEFTCORNER+BOARDWIDTH+10;
const int NEXTBLOCK_TOPCORNER =TOPCORNER+BOARDHEIGHT-200;
const int NB_COLS_NEXTBLOCK =6;
const int NB_ROWS_NEXTBLOCK =6;


const int LEVEL_LEFTCORNER = 20;
const int LEVEL_TOPCORNER = 20;
const int LEVEL_RIGHTCORNER = LEVEL_LEFTCORNER+8.5*CELL_SIZE;
const int LEVEL_BOTTOMCORNER = LEVEL_TOPCORNER+8*CELL_SIZE;

const int LEFTARROW=75;
const int RIGHTARROW=77;
const int UPARROW=72;
const int DOWNARROW=80;
const int ENTER=13;
const int ESC=27;
const int SPACE=32;
int lev=1;

void Clear()
{
    while (kbhit())
        getch();
}

int highscore(char* file)
{
    fstream f;
    int hs;
    f.open(file, ios::out|ios::app|ios::in);
    int scores[150]={};//will introduce DMM soon
    int i=0;
    while(!f.eof())
    {
        f>>scores[i];
        i++;
    }
    hs=scores[0];
    for(int j=1;j<i;j++)
        if(scores[j]>hs)
            hs=scores[j];
    return hs;
}
bool highScore(int score, char* file)
{
    bool highestScore=0;
    int hs=highscore(file);
    if(score>hs)
        highestScore=1;
    return highestScore;
}

bool Scoreinput(int score)
{
    bool scoreHS;
    fstream scc;
    scc.open("TetrisScore.txt", ios::out|ios::app);
    scoreHS=highScore(score,"TetrisScore.txt");
    scc<<score<<endl;
    return scoreHS;
}


void initGraphEngine()
{
    int gdriver = DETECT, gmode;
    initgraph(&gdriver, &gmode, "...\\BGI");
}

void displayInfo(char *msg)
{
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, BLUE);
    bar(INFOAREALEFT+1,INFOAREATOP+1,INFOAREALEFT+500,
        INFOAREATOP+INFOAREAHEIGHT-1);
    settextstyle(BOLD_FONT, HORIZ_DIR, 1);
    outtextxy(INFOAREALEFT+75,INFOAREATOP+50,msg);
}

void clearDisplayArea()
{
    setfillstyle(SOLID_FILL, BLACK);
    bar(LEFTCORNER+CELL_SIZE,0,(LEFTCORNER+BOARDWIDTH-CELL_SIZE),TOPCORNER-1);
}

void fillGrid(int leftcorner, int topcorner, int nbrows, int nbcols, int color)
{
    int x = leftcorner+CELL_SIZE,  y = topcorner;
    setcolor(color);
    for(int i=1; i<=nbrows; i++)
    {
        for (int j=1; j<=nbcols; j++)
        {
            rectangle(x,y,x+CELL_SIZE,y+CELL_SIZE);
            x+=CELL_SIZE;
        }
        x=leftcorner+CELL_SIZE;
        y+=CELL_SIZE;
    }
}

void drawLevels(int x)
{
    char arr[50];
    sprintf(arr, "LEVEL: %d",x);
    setcolor(WHITE);
    settextstyle(BOLD_FONT,HORIZ_DIR,1);
    outtextxy(LEVEL_LEFTCORNER+CELL_SIZE/4,LEVEL_TOPCORNER+CELL_SIZE, arr);
}

void drawScore(int x)
{
    char arr[50];
    sprintf(arr, "SCORE: %d",x);
    setcolor(WHITE);
    settextstyle(BOLD_FONT,HORIZ_DIR,1);
    outtextxy(LEVEL_LEFTCORNER+CELL_SIZE/4,LEVEL_TOPCORNER+2*CELL_SIZE, arr);
}

void drawCurrentHighScore()
{
    int curHighScore=highscore("TetrisScore.txt");
    char arr[50];
    sprintf(arr, "HIGH SCORE: %d",curHighScore);
    setcolor(WHITE);
    settextstyle(BOLD_FONT,HORIZ_DIR,1);
    outtextxy(LEVEL_LEFTCORNER+CELL_SIZE/4,LEVEL_TOPCORNER+3*CELL_SIZE, arr);
}

void drawLevelsBox()
{
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, BLACK);
    bar(LEVEL_LEFTCORNER,LEVEL_TOPCORNER,LEVEL_RIGHTCORNER,LEVEL_BOTTOMCORNER);
    settextstyle(BOLD_FONT,HORIZ_DIR,1);
    outtextxy(LEVEL_LEFTCORNER+CELL_SIZE/4,LEVEL_TOPCORNER+CELL_SIZE, "LEVEL: ");
    outtextxy(LEVEL_LEFTCORNER+CELL_SIZE/4,LEVEL_TOPCORNER+2*CELL_SIZE, "SCORE: ");
    drawCurrentHighScore();
}


void drawBoard()
{
    setfillstyle(SOLID_FILL, WHITE);
    bar(LEFTCORNER,TOPCORNER,LEFTCORNER+BOARDWIDTH,TOPCORNER+BOARDHEIGHT);
    setfillstyle(SOLID_FILL, BOARD_COLOR);
    bar(LEFTCORNER+CELL_SIZE,TOPCORNER,(LEFTCORNER+BOARDWIDTH-CELL_SIZE),(TOPCORNER+BOARDHEIGHT-CELL_SIZE));
    fillGrid(LEFTCORNER,TOPCORNER,NB_ROWS,NB_COLS,GRID_COLOR);
    setfillstyle(SOLID_FILL, BLUE);
    bar(INFOAREALEFT,INFOAREATOP,INFOAREALEFT+INFOAREAWIDTH,INFOAREATOP+INFOAREAHEIGHT);
    rectangle(INFOAREALEFT,INFOAREATOP,INFOAREALEFT+INFOAREAWIDTH,INFOAREATOP+INFOAREAHEIGHT);
    drawLevelsBox();
}



void reDrawCellsGrid(int xLeft, int yTop, int nbCells, int orientation)
{
    int i;
    setcolor(BORDER_COLOR);
    for (i=1; i<=nbCells; i++)
    {
        rectangle(xLeft, yTop, xLeft+CELL_SIZE, yTop+CELL_SIZE);
        if (orientation == HORIZONTAL)  xLeft=xLeft+CELL_SIZE;
        else yTop = yTop + CELL_SIZE;
    }
}


void drawBarBlock(int xLeft, int yTop, int orientation, int fillingColor)
{
    setfillstyle(SOLID_FILL, fillingColor);
    if (orientation==HORIZONTAL)
    {
        bar(xLeft, yTop, xLeft+4*CELL_SIZE,yTop+CELL_SIZE);
        reDrawCellsGrid(xLeft, yTop, 4, HORIZONTAL);
    }
    else
    {
        bar(xLeft, yTop, xLeft+CELL_SIZE, yTop+4*CELL_SIZE);
        reDrawCellsGrid(xLeft, yTop ,4, VERTICAL);
    }
}

void drawLRblock(int xLeft, int yTop, int orientation, int fillingColor)
{
    setfillstyle(SOLID_FILL, fillingColor);
    if(orientation==NORTH || orientation==SOUTH)
    {
        bar(xLeft,yTop,xLeft+CELL_SIZE,yTop+2*CELL_SIZE);
        bar(xLeft+CELL_SIZE,yTop+CELL_SIZE,xLeft+2*CELL_SIZE,yTop+3*CELL_SIZE);
        reDrawCellsGrid(xLeft,yTop,2,VERTICAL);
        reDrawCellsGrid(xLeft+CELL_SIZE,yTop+CELL_SIZE,2,VERTICAL);
    }

    else
    {
        bar(xLeft,yTop,xLeft+2*CELL_SIZE,yTop+CELL_SIZE);
        bar(xLeft-CELL_SIZE,yTop+CELL_SIZE,xLeft+CELL_SIZE,yTop+2*CELL_SIZE);
        reDrawCellsGrid(xLeft,yTop,2,HORIZONTAL);
        reDrawCellsGrid(xLeft-CELL_SIZE,yTop+CELL_SIZE,2,HORIZONTAL);
    }
}
void drawTblock(int xLeft, int yTop, int orientation, int fillingColor)
{
    setfillstyle(SOLID_FILL,fillingColor);
    if(orientation==NORTH)
    {
        bar(xLeft,yTop,xLeft+3*CELL_SIZE,yTop+CELL_SIZE);
        bar(xLeft+CELL_SIZE,yTop+CELL_SIZE,xLeft+2*CELL_SIZE,yTop+2*CELL_SIZE);
        reDrawCellsGrid(xLeft+CELL_SIZE,yTop+CELL_SIZE,1,HORIZONTAL);
        reDrawCellsGrid(xLeft,yTop,3,HORIZONTAL);
    }
    else if(orientation==SOUTH)
    {
        bar(xLeft,yTop,xLeft+CELL_SIZE,yTop+CELL_SIZE);
        bar(xLeft-CELL_SIZE,yTop+CELL_SIZE,xLeft+2*CELL_SIZE,yTop+2*CELL_SIZE);
        reDrawCellsGrid(xLeft,yTop,1,HORIZONTAL);
        reDrawCellsGrid(xLeft-CELL_SIZE,yTop+CELL_SIZE,3,HORIZONTAL);
    }
    else if(orientation==EAST)
    {
        bar(xLeft,yTop,xLeft+CELL_SIZE,yTop+3*CELL_SIZE);
        bar(xLeft+CELL_SIZE,yTop+CELL_SIZE,xLeft+2*CELL_SIZE,yTop+2*CELL_SIZE);
        reDrawCellsGrid(xLeft,yTop,3,VERTICAL);
        reDrawCellsGrid(xLeft+CELL_SIZE,yTop+CELL_SIZE,1,HORIZONTAL);
    }
    else if(orientation==WEST)
    {
        bar(xLeft,yTop,xLeft+CELL_SIZE,yTop+3*CELL_SIZE);
        bar(xLeft-CELL_SIZE,yTop+CELL_SIZE,xLeft,yTop+2*CELL_SIZE);
        reDrawCellsGrid(xLeft,yTop,3,VERTICAL);
        reDrawCellsGrid(xLeft-CELL_SIZE,yTop+CELL_SIZE,1,HORIZONTAL);
    }
}
void drawBoxblock(int xLeft, int yTop, int orientation, int fillingColor)
{
    setfillstyle(SOLID_FILL, fillingColor);
    bar(xLeft,yTop,xLeft+2*CELL_SIZE,yTop+CELL_SIZE);
    bar(xLeft,yTop+CELL_SIZE,xLeft+2*CELL_SIZE,yTop+2*CELL_SIZE);
    reDrawCellsGrid(xLeft,yTop,2,HORIZONTAL);
    reDrawCellsGrid(xLeft,yTop+CELL_SIZE,2,HORIZONTAL);
}
void drawLblock(int xLeft, int yTop, int orientation, int fillingColor)
{
    setfillstyle(SOLID_FILL,fillingColor);
    if(orientation==NORTH)
    {
        bar(xLeft,yTop,xLeft+CELL_SIZE,yTop+3*CELL_SIZE);
        bar(xLeft+CELL_SIZE,yTop+2*CELL_SIZE,xLeft+2*CELL_SIZE,yTop+3*CELL_SIZE);
        reDrawCellsGrid(xLeft,yTop,3,VERTICAL);
        reDrawCellsGrid(xLeft+CELL_SIZE,yTop+2*CELL_SIZE,1,HORIZONTAL);
    }
    else if(orientation==SOUTH)
    {
        bar(xLeft,yTop,xLeft+CELL_SIZE,yTop+CELL_SIZE);
        bar(xLeft+CELL_SIZE,yTop,xLeft+2*CELL_SIZE,yTop+3*CELL_SIZE);
        reDrawCellsGrid(xLeft,yTop,1,HORIZONTAL);
        reDrawCellsGrid(xLeft+CELL_SIZE,yTop,3,VERTICAL);
    }
    else if(orientation==EAST)
    {
        bar(xLeft,yTop,xLeft+3*CELL_SIZE,yTop+CELL_SIZE);
        bar(xLeft,yTop+CELL_SIZE,xLeft+CELL_SIZE,yTop+2*CELL_SIZE);
        reDrawCellsGrid(xLeft,yTop,3,HORIZONTAL);
        reDrawCellsGrid(xLeft,yTop+CELL_SIZE,1,HORIZONTAL);
    }
    else if(orientation==WEST)
    {
        bar(xLeft,yTop,xLeft+CELL_SIZE,yTop+CELL_SIZE);
        bar(xLeft-2*CELL_SIZE,yTop+CELL_SIZE,xLeft+CELL_SIZE,yTop+2*CELL_SIZE);
        reDrawCellsGrid(xLeft,yTop,1,VERTICAL);
        reDrawCellsGrid(xLeft-2*CELL_SIZE,yTop+CELL_SIZE,3,HORIZONTAL);
    }
}
void generateBarBlock(int & orientation, int & xLeft, int & yTop, int & fillingColor)
{
    orientation = rand()%2 + 1;
    fillingColor = rand()%13 + 1;
    yTop = TOPCORNER;
    if (orientation == HORIZONTAL) xLeft  = LEFTCORNER+4*CELL_SIZE;
    else    xLeft = LEFTCORNER+5*CELL_SIZE;
    drawBarBlock(xLeft,yTop,orientation, fillingColor /*FRGRD_COLOR*/);
}

void generateLRblock(int &orientation, int &xLeft, int &yTop, int &fillingcolor)
{
    orientation=rand()%4 +1;
    fillingcolor=rand()%13 +1;
    yTop=TOPCORNER;
    xLeft=LEFTCORNER+5*CELL_SIZE;
    drawLRblock(xLeft,yTop,orientation,fillingcolor);
}
void generateTblock(int &orientation, int &xLeft, int &yTop, int &fillingcolor)
{
    orientation=rand()%4 +1;
    fillingcolor=rand()%13 +1;
    yTop=TOPCORNER;
    xLeft=LEFTCORNER+5*CELL_SIZE;
    drawTblock(xLeft,yTop,orientation,fillingcolor);
}

void generateBoxblock(int &orientation, int &xLeft, int &yTop, int &fillingcolor)
{
    orientation=1;
    fillingcolor=rand()%13 +1;
    yTop=TOPCORNER;
    xLeft=LEFTCORNER+5*CELL_SIZE;
    drawBoxblock(xLeft,yTop,orientation,fillingcolor);

}
void generateLblock(int &orientation, int &xLeft, int &yTop, int &fillingcolor)
{
    orientation=rand()%4 +1;
    fillingcolor=rand()%13 +1;
    yTop=TOPCORNER;
    xLeft=LEFTCORNER+5*CELL_SIZE;
    drawLblock(xLeft,yTop,orientation,fillingcolor);
}
void drawNextBlock(int xLeft, int yTop, int block)
{

    int orientation=1, fillingColor=YELLOW;
    switch(block)
    {
    case T_BLOCK  :
        drawTblock(xLeft,yTop,orientation,fillingColor);
        break;
    case L_BLOCK  :
        drawLblock(xLeft,yTop,orientation,fillingColor);
        break;
    case BAR_BLOCK :
        drawBarBlock(xLeft,yTop,orientation,fillingColor);
        break;
    case BOX_BLOCK:
        drawBoxblock(xLeft,yTop,orientation,fillingColor);
        break;
    case LR_BLOCK:
        drawLRblock(xLeft, yTop, orientation, fillingColor);
        break;
    }
}

int drawNextBlockArea()
{
    int block;
    block= rand()%5 +1;
    setcolor(BKGRD_COLOR);
    setfillstyle(SOLID_FILL,BKGRD_COLOR);
    bar(NEXTBLOCK_LEFTCORNER,NEXTBLOCK_TOPCORNER,NEXTBLOCK_LEFTCORNER+6*CELL_SIZE,NEXTBLOCK_TOPCORNER+6*CELL_SIZE);
    fillGrid(NEXTBLOCK_LEFTCORNER-CELL_SIZE, NEXTBLOCK_TOPCORNER, NB_ROWS_NEXTBLOCK, NB_COLS_NEXTBLOCK, BKGRD_COLOR);
    setcolor(WHITE);
    settextstyle(BOLD_FONT,HORIZ_DIR,1);
    outtextxy(NEXTBLOCK_LEFTCORNER+CELL_SIZE,NEXTBLOCK_TOPCORNER-CELL_SIZE,"Next Block:");
    switch(block)
    {
    case T_BLOCK  :
        drawNextBlock(NEXTBLOCK_LEFTCORNER+CELL_SIZE, NEXTBLOCK_TOPCORNER+2*CELL_SIZE, block);
        break;
    case L_BLOCK  :
        drawNextBlock(NEXTBLOCK_LEFTCORNER+CELL_SIZE, NEXTBLOCK_TOPCORNER+2*CELL_SIZE, block);
        break;
    case BAR_BLOCK :
        drawNextBlock(NEXTBLOCK_LEFTCORNER+CELL_SIZE,NEXTBLOCK_TOPCORNER+2*CELL_SIZE, block);
        break;
    case BOX_BLOCK:
        drawNextBlock(NEXTBLOCK_LEFTCORNER+CELL_SIZE, NEXTBLOCK_TOPCORNER+2*CELL_SIZE, block);
        break;
    case LR_BLOCK:
        drawNextBlock(NEXTBLOCK_LEFTCORNER+CELL_SIZE, NEXTBLOCK_TOPCORNER+2*CELL_SIZE, block);
        break;
    }
    return block;

}


void generateNewBlock(int * blockId, int * orientation,int * xLeft, int *yTop, int * fillingColor)
{
    static int y;
    static int x=1;
    if(x==1)
    {
        *blockId= rand()%5 +1;
        x++;
    }
    else
        *blockId = y;
    switch(*blockId)
    {
    case T_BLOCK  :
        generateTblock(*orientation, *xLeft, *yTop, *fillingColor);
        break;
    case L_BLOCK  :
        generateLblock(*orientation, *xLeft, *yTop, *fillingColor);
        break;
    case LR_BLOCK :
        generateLRblock(*orientation,*xLeft,*yTop,*fillingColor);
        break;
    case BOX_BLOCK:
        generateBoxblock(*orientation,*xLeft,*yTop,*fillingColor);
        break;
    case BAR_BLOCK:
        generateBarBlock(*orientation, *xLeft, *yTop, *fillingColor);
        break;
    }
    y= drawNextBlockArea();
}

void checkTranslateBarRouteLeft(int orientation, int xLeft, int yTop, int &freeRoute)
{
    int maxNbCells;
    yTop = yTop + CELL_SIZE/2;
    freeRoute = 1;
    switch (orientation)
    {
    case HORIZONTAL:
        xLeft = xLeft - CELL_SIZE/2;
        if (getpixel(xLeft, yTop) != BOARD_COLOR) freeRoute = 0;
        break;

    case VERTICAL:
        maxNbCells = 4;
        xLeft  = xLeft - CELL_SIZE/2;
        do
            if (getpixel(xLeft, yTop) == BOARD_COLOR )
            {
                yTop = yTop + CELL_SIZE;
                maxNbCells--;
            }
            else  freeRoute = 0;
        while ( maxNbCells >= 1 && freeRoute);
        break;
    }
}

void checkTranslateBarRouteRight(int orientation, int xLeft, int yTop, int &freeRoute)
{
    int maxNbCells;

    freeRoute = 1;
    switch (orientation)
    {
    case HORIZONTAL:
        yTop = yTop + CELL_SIZE/2;
        xLeft = xLeft + 4*CELL_SIZE + CELL_SIZE/2;
        if (getpixel(xLeft, yTop) != BOARD_COLOR)
            freeRoute = 0;
        break;

    case VERTICAL:
        maxNbCells = 4;
        yTop = yTop + CELL_SIZE/2;
        xLeft  = xLeft + CELL_SIZE + CELL_SIZE/2;
        do
            if (getpixel(xLeft, yTop) == BOARD_COLOR )
            {
                yTop = yTop + CELL_SIZE;
                maxNbCells--;
            }
            else  freeRoute = 0;
        while ( maxNbCells >= 1 && freeRoute);
        break;
    }
}

void checkTranslateBarRouteDown(int orientation, int xLeft, int yTop, int &freeRoute)
{
    int maxNbCells;
    xLeft = xLeft + CELL_SIZE/2;
    freeRoute = 1;
    switch(orientation)
    {
    case HORIZONTAL:
        maxNbCells = 4;
        yTop  = yTop + CELL_SIZE + CELL_SIZE/2;
        do
            if (getpixel(xLeft, yTop) == BOARD_COLOR )
            {
                xLeft = xLeft + CELL_SIZE;
                maxNbCells--;
            }
            else  freeRoute = 0;
        while ( maxNbCells >= 1 && freeRoute);
        break;

    case VERTICAL:
        yTop = yTop + 4*CELL_SIZE + CELL_SIZE/2;
        if (getpixel(xLeft, yTop) != BOARD_COLOR) freeRoute = 0;
        break;
    }
}

int checkTranslateBarRoute(int orientation, int xLeft, int yTop,int direction)
{
    int maxNbCells, freeRoute = 1;
    switch (direction)
    {
    case RIGHT:
        checkTranslateBarRouteRight(orientation, xLeft, yTop,freeRoute);
        break;
    case LEFT:
        checkTranslateBarRouteLeft(orientation, xLeft, yTop,freeRoute);
        break;
    case DOWN:
        checkTranslateBarRouteDown(orientation, xLeft, yTop,freeRoute);
        break;
    }
    return freeRoute;
}

void translateBarBlock(int orientation, int & xLeft, int & yTop, int fillingColor,int direction)
{
    drawBarBlock(xLeft, yTop, orientation, BKGRD_COLOR);
    switch (direction)
    {
    case RIGHT:
        xLeft = xLeft + CELL_SIZE;
        break;
    case LEFT :
        xLeft = xLeft - CELL_SIZE;
        break;
    case DOWN :
        yTop = yTop + CELL_SIZE;
        break;
    }
    drawBarBlock(xLeft, yTop, orientation, fillingColor);
}

void checktranslateLRblockright(int orientation, int xLeft, int yTop, int &freeRoute)
{
    yTop=yTop+CELL_SIZE/2;
    freeRoute=1;
    if(orientation==NORTH || orientation==SOUTH)
    {
        xLeft=xLeft+CELL_SIZE+CELL_SIZE/2;
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            yTop+=CELL_SIZE;
            xLeft+=CELL_SIZE;
            if(getpixel(xLeft,yTop)==BOARD_COLOR)
            {
                yTop+=CELL_SIZE;
                if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                    freeRoute=0;
            }
            else
                freeRoute=0;
        }
        else
            freeRoute=0;
    }
    else
    {
        xLeft=xLeft+2*CELL_SIZE+CELL_SIZE/2;
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            xLeft-=CELL_SIZE;
            yTop+=CELL_SIZE;
            if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                freeRoute=0;
        }
        else
            freeRoute=0;
    }
}

void checktranslateLRblockleft(int orientation, int xLeft, int yTop, int &freeRoute)
{
    yTop=yTop+CELL_SIZE/2;
    freeRoute=1;
    xLeft=xLeft-CELL_SIZE/2;
    if(orientation==NORTH || orientation==SOUTH)
    {
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            yTop+=CELL_SIZE;
            if(getpixel(xLeft,yTop)==BOARD_COLOR)
            {
                xLeft+=CELL_SIZE;
                yTop+=CELL_SIZE;
                if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                    freeRoute=0;
            }
            else
                freeRoute=0;
        }
        else
            freeRoute=0;
    }
    else
    {
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            xLeft-=CELL_SIZE;
            yTop+=CELL_SIZE;
            if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                freeRoute=0;
        }
        else
            freeRoute=0;
    }
}

void checktranslateLRblockdown(int orientation, int xLeft, int yTop, int &freeRoute)
{
    freeRoute=1;
    xLeft=xLeft+CELL_SIZE/2;
    if(orientation==NORTH || orientation==SOUTH)
    {
        yTop=yTop+2*CELL_SIZE+CELL_SIZE/2;
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            yTop+=CELL_SIZE;
            xLeft+=CELL_SIZE;
            if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                freeRoute=0;
        }
        else
            freeRoute=0;
    }
    else
    {
        yTop=yTop+2*CELL_SIZE+CELL_SIZE/2;
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            xLeft-=CELL_SIZE;
            if(getpixel(xLeft,yTop)==BOARD_COLOR)
            {
                xLeft+=2*CELL_SIZE;
                yTop-=CELL_SIZE;
                if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                    freeRoute=0;
            }
            else freeRoute=0;
        }
        else
            freeRoute=0;
    }
}

int checktranslateLRroute(int orientation, int xLeft, int yTop, int direction)
{
    int freeRoute;
    switch(direction)
    {
    case LEFT:
        checktranslateLRblockleft(orientation,xLeft,yTop,freeRoute);
        break;
    case RIGHT:
        checktranslateLRblockright(orientation,xLeft,yTop,freeRoute);
        break;
    case DOWN:
        checktranslateLRblockdown(orientation,xLeft,yTop,freeRoute);
        break;
    }
    return freeRoute;
}

void translateLRblock(int orientation, int &xLeft, int &yTop, int fillingcolor, int direction)
{
    drawLRblock(xLeft, yTop, orientation, BKGRD_COLOR);
    switch(direction)
    {
    case LEFT:
        xLeft=xLeft-CELL_SIZE;
        break;
    case RIGHT:
        xLeft=xLeft+CELL_SIZE;
        break;
    case DOWN:
        yTop=yTop+CELL_SIZE;
        break;
    }
    drawLRblock(xLeft,yTop,orientation,fillingcolor);
}
void checktranslateTblockright(int orientation, int xLeft, int yTop, int &freeRoute)
{
    yTop=yTop+CELL_SIZE/2;
    freeRoute=1;
    if(orientation==NORTH)
    {
        xLeft=xLeft+3*CELL_SIZE+CELL_SIZE/2;
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            yTop+=CELL_SIZE;
            xLeft-=CELL_SIZE;
            if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                freeRoute=0;
        }
        else
            freeRoute=0;
    }
    else if(orientation==SOUTH)
    {
        xLeft=xLeft+CELL_SIZE+CELL_SIZE/2;
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            xLeft+=CELL_SIZE;
            yTop+=CELL_SIZE;
            if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                freeRoute=0;
        }
        else
            freeRoute=0;
    }
    else if(orientation==EAST)
    {
        xLeft=xLeft+CELL_SIZE+CELL_SIZE/2;
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            xLeft+=CELL_SIZE;
            yTop+=CELL_SIZE;
            if(getpixel(xLeft,yTop)==BOARD_COLOR)
            {
                xLeft-=CELL_SIZE;
                yTop+=CELL_SIZE;
                if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                    freeRoute=0;
            }
            else
                freeRoute=0;
        }
        else
            freeRoute=0;
    }
    else if(orientation=WEST)
    {
        xLeft=xLeft+CELL_SIZE+CELL_SIZE/2;
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            yTop+=CELL_SIZE;
            if(getpixel(xLeft,yTop)==BOARD_COLOR)
            {
                yTop+=CELL_SIZE;
                if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                    freeRoute=0;
            }
            else
                freeRoute=0;
        }
        else
            freeRoute=0;
    }
}

void checktranslateTblockleft(int orientation, int xLeft, int yTop, int &freeRoute)
{
    yTop=yTop+CELL_SIZE/2;
    freeRoute=1;
    xLeft=xLeft-CELL_SIZE/2;
    if(orientation==NORTH)
    {
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            yTop+=CELL_SIZE;
            xLeft+=CELL_SIZE;
            if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                freeRoute=0;
        }
        else
            freeRoute=0;
    }
    else if(orientation==SOUTH)
    {
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            xLeft-=CELL_SIZE;
            yTop+=CELL_SIZE;
            if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                freeRoute=0;
        }
        else
            freeRoute=0;
    }
    else if(orientation==EAST)
    {
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            yTop+=CELL_SIZE;
            if(getpixel(xLeft,yTop)==BOARD_COLOR)
            {
                yTop+=CELL_SIZE;
                if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                    freeRoute=0;
            }
            else
                freeRoute=0;
        }
        else
            freeRoute=0;
    }
    else if(orientation==WEST)
    {
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            yTop+=CELL_SIZE;
            xLeft-=CELL_SIZE;
            if(getpixel(xLeft,yTop)==BOARD_COLOR)
            {
                xLeft+=CELL_SIZE;
                yTop+=CELL_SIZE;
                if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                    freeRoute=0;
            }
            else
                freeRoute=0;
        }
        else
            freeRoute=0;
    }
}

void checktranslateTblockdown(int orientation, int xLeft, int yTop, int &freeRoute)
{
    freeRoute=1;
    xLeft=xLeft+CELL_SIZE/2;
    if(orientation==NORTH)
    {
        yTop=yTop+CELL_SIZE+CELL_SIZE/2;
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            yTop+=CELL_SIZE;
            xLeft+=CELL_SIZE;
            if(getpixel(xLeft,yTop)==BOARD_COLOR)
            {
                yTop-=CELL_SIZE;
                xLeft+=CELL_SIZE;
                if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                    freeRoute=0;
            }
            else
                freeRoute=0;
        }
        else
            freeRoute=0;
    }
    else if(orientation==SOUTH)
    {
        yTop=yTop+2*CELL_SIZE+CELL_SIZE/2;
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            xLeft-=CELL_SIZE;
            if(getpixel(xLeft,yTop)==BOARD_COLOR)
            {
                xLeft=xLeft+2*CELL_SIZE;
                if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                    freeRoute=0;
            }
            else
                freeRoute=0;
        }
        else
            freeRoute=0;
    }
    else if(orientation==EAST)
    {
        yTop=yTop+3*CELL_SIZE+CELL_SIZE/2;
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            yTop-=CELL_SIZE;
            xLeft+=CELL_SIZE;
            if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                freeRoute=0;
        }
        else
            freeRoute=0;
    }
    else if(orientation==WEST)
    {
        yTop=yTop+3*CELL_SIZE+CELL_SIZE/2;
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            yTop-=CELL_SIZE;
            xLeft-=CELL_SIZE;
            if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                freeRoute=0;
        }
        else
            freeRoute=0;
    }
}

int checktranslateTroute(int orientation, int xLeft, int yTop, int direction)
{
    int freeRoute;
    switch(direction)
    {
    case LEFT:
        checktranslateTblockleft(orientation,xLeft,yTop,freeRoute);
        break;
    case RIGHT:
        checktranslateTblockright(orientation,xLeft,yTop,freeRoute);
        break;
    case DOWN:
        checktranslateTblockdown(orientation,xLeft,yTop,freeRoute);
        break;
    }
    return freeRoute;
}

void translateTblock(int orientation, int &xLeft, int &yTop, int fillingcolor, int direction)
{
    drawTblock(xLeft, yTop, orientation, BKGRD_COLOR);
    switch(direction)
    {
    case LEFT:
        xLeft=xLeft-CELL_SIZE;
        break;
    case RIGHT:
        xLeft=xLeft+CELL_SIZE;
        break;
    case DOWN:
        yTop=yTop+CELL_SIZE;
        break;
    }
    drawTblock(xLeft,yTop,orientation,fillingcolor);
}

void checktranslateBoxblockright(int orientation, int xLeft, int yTop, int &freeRoute)
{
    yTop+=CELL_SIZE/2;
    xLeft=xLeft+2*CELL_SIZE+CELL_SIZE/2;
    freeRoute=1;
    if(getpixel(xLeft,yTop)==BOARD_COLOR)
    {
        yTop+=CELL_SIZE;
        if(getpixel(xLeft,yTop)!=BOARD_COLOR)
            freeRoute=0;
    }
    else
        freeRoute=0;
}

void checktranslateBoxblockleft(int orientation, int xLeft, int yTop, int &freeRoute)
{
    yTop+=CELL_SIZE/2;
    xLeft-=CELL_SIZE/2;
    freeRoute=1;
    if(getpixel(xLeft,yTop)==BOARD_COLOR)
    {
        yTop+=CELL_SIZE;
        if(getpixel(xLeft,yTop)!=BOARD_COLOR)
            freeRoute=0;
    }
    else
        freeRoute=0;

}

void checktranslateBoxblockdown(int orientation, int xLeft, int yTop, int &freeRoute)
{
    xLeft+=CELL_SIZE/2;
    yTop=yTop+2*CELL_SIZE+CELL_SIZE/2;
    freeRoute=1;
    if(getpixel(xLeft,yTop)==BOARD_COLOR)
    {
        xLeft+=CELL_SIZE;
        if(getpixel(xLeft,yTop)!=BOARD_COLOR)
            freeRoute=0;
    }
    else
        freeRoute=0;

}

int checktranslateBoxroute(int orientation, int xLeft, int yTop, int direction)
{
    int freeRoute;
    switch(direction)
    {
    case LEFT:
        checktranslateBoxblockleft(orientation,xLeft,yTop,freeRoute);
        break;
    case RIGHT:
        checktranslateBoxblockright(orientation,xLeft,yTop,freeRoute);
        break;
    case DOWN:
        checktranslateBoxblockdown(orientation,xLeft,yTop,freeRoute);
        break;
    }
    return freeRoute;
}

void translateBoxblock(int orientation, int &xLeft, int &yTop, int fillingcolor, int direction)
{
    drawBoxblock(xLeft, yTop, orientation, BKGRD_COLOR);
    switch(direction)
    {
    case LEFT:
        xLeft-=CELL_SIZE;
        break;
    case RIGHT:
        xLeft+=CELL_SIZE;
        break;
    case DOWN:
        yTop+=CELL_SIZE;
        break;
    }
    drawBoxblock(xLeft,yTop,orientation,fillingcolor);
}

void checktranslateLblockright(int orientation, int xLeft, int yTop, int &freeRoute)
{
    yTop=yTop+CELL_SIZE/2;
    freeRoute=1;
    if(orientation==NORTH)
    {
        xLeft=xLeft+CELL_SIZE+CELL_SIZE/2;
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            yTop+=CELL_SIZE;
            if(getpixel(xLeft,yTop)==BOARD_COLOR)
            {
                yTop+=CELL_SIZE;
                xLeft+=CELL_SIZE;
                if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                    freeRoute=0;
            }
            else
                freeRoute=0;
        }
        else
            freeRoute=0;
    }
    else if(orientation==SOUTH)
    {
        xLeft=xLeft+2*CELL_SIZE+CELL_SIZE/2;
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            yTop+=CELL_SIZE;
            if(getpixel(xLeft,yTop)==BOARD_COLOR)
            {
                yTop+=CELL_SIZE;
                if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                    freeRoute=0;
            }
            else
                freeRoute=0;
        }
        else
            freeRoute=0;
    }
    else if(orientation==EAST)
    {
        xLeft=xLeft+3*CELL_SIZE+CELL_SIZE/2;
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            xLeft-=CELL_SIZE;
            yTop+=CELL_SIZE;
            if(getpixel(xLeft,yTop)==BOARD_COLOR)
            {
                xLeft-=CELL_SIZE;
                if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                    freeRoute=0;
            }
            else
                freeRoute=0;
        }
        else
            freeRoute=0;
    }
    else if(orientation=WEST)
    {
        xLeft=xLeft+CELL_SIZE+CELL_SIZE/2;
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            yTop+=CELL_SIZE;
            if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                freeRoute=0;
        }
        else
            freeRoute=0;
    }
}

void checktranslateLblockleft(int orientation, int xLeft, int yTop, int &freeRoute)
{
    freeRoute=1;
    yTop=yTop+CELL_SIZE/2;
    xLeft=xLeft-CELL_SIZE/2;
    if(orientation==NORTH)
    {
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            yTop+=CELL_SIZE;
            if(getpixel(xLeft,yTop)==BOARD_COLOR)
            {
                yTop+=CELL_SIZE;
                if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                    freeRoute=0;
            }
            else
                freeRoute=0;
        }
        else
            freeRoute=0;
    }
    else if(orientation==SOUTH)
    {
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            yTop+=CELL_SIZE;
            xLeft+=CELL_SIZE;
            if(getpixel(xLeft,yTop)==BOARD_COLOR)
            {
                yTop+=CELL_SIZE;
                if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                    freeRoute=0;
            }
            else
                freeRoute=0;
        }
        else
            freeRoute=0;
    }
    else if(orientation==EAST)
    {
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            yTop+=CELL_SIZE;
            if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                freeRoute=0;
        }
        else
            freeRoute=0;
    }
    else if(orientation==WEST)
    {
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            xLeft-=CELL_SIZE;
            if(getpixel(xLeft,yTop)==BOARD_COLOR)
            {
                xLeft-=CELL_SIZE;
                yTop+=CELL_SIZE;
                if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                    freeRoute=0;
            }
            else
                freeRoute=0;
        }
        else
            freeRoute=0;
    }
}

void checktranslateLblockdown(int orientation, int xLeft, int yTop, int &freeRoute)
{
    freeRoute=1;
    xLeft=xLeft+CELL_SIZE/2;
    if(orientation==NORTH)
    {
        yTop=yTop+3*CELL_SIZE+CELL_SIZE/2;
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            xLeft+=CELL_SIZE;
            if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                freeRoute=0;
        }
        else
            freeRoute=0;
    }
    else if(orientation==SOUTH)
    {
        yTop=yTop+CELL_SIZE+CELL_SIZE/2;
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            yTop=yTop+2*CELL_SIZE;
            xLeft+=CELL_SIZE;
            if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                freeRoute=0;
        }
        else
            freeRoute=0;
    }
    else if(orientation==EAST)
    {
        yTop=yTop+2*CELL_SIZE+CELL_SIZE/2;
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            yTop-=CELL_SIZE;
            xLeft+=CELL_SIZE;
            if(getpixel(xLeft,yTop)==BOARD_COLOR)
            {
                xLeft+=CELL_SIZE;
                if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                    freeRoute=0;
            }
            else
                freeRoute=0;
        }
        else
            freeRoute=0;
    }
    else if(orientation==WEST)
    {
        yTop=yTop+2*CELL_SIZE+CELL_SIZE/2;
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            xLeft-=CELL_SIZE;
            if(getpixel(xLeft,yTop)==BOARD_COLOR)
            {
                xLeft-=CELL_SIZE;
                if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                    freeRoute=0;
            }
            else
                freeRoute=0;
        }
        else
            freeRoute=0;

    }
}

int checktranslateLroute(int orientation, int xLeft, int yTop, int direction)
{
    int freeRoute;
    switch(direction)
    {
    case LEFT:
        checktranslateLblockleft(orientation,xLeft,yTop,freeRoute);
        break;
    case RIGHT:
        checktranslateLblockright(orientation,xLeft,yTop,freeRoute);
        break;
    case DOWN:
        checktranslateLblockdown(orientation,xLeft,yTop,freeRoute);
        break;
    }
    return freeRoute;
}

void translateLblock(int orientation, int &xLeft, int &yTop, int fillingcolor, int direction)
{
    drawLblock(xLeft, yTop, orientation, BKGRD_COLOR);
    switch(direction)
    {
    case LEFT:
        xLeft=xLeft-CELL_SIZE;
        break;
    case RIGHT:
        xLeft=xLeft+CELL_SIZE;
        break;
    case DOWN:
        yTop=yTop+CELL_SIZE;
        break;
    }
    drawLblock(xLeft,yTop,orientation,fillingcolor);
}

int translateRouteFree(int blockId, int orientation, int xLeft, int yTop,
                       int direction)
{
    int freeRoute;
    switch(blockId)
    {
    case T_BLOCK  :
        freeRoute=checktranslateTroute(orientation, xLeft, yTop, direction);
        break;
    case L_BLOCK  :
        freeRoute=checktranslateLroute(orientation, xLeft, yTop, direction);
        break;
    case LR_BLOCK :
        freeRoute=checktranslateLRroute(orientation,xLeft,yTop,direction);
        break;
    case BOX_BLOCK:
        freeRoute=checktranslateBoxroute(orientation,xLeft,yTop,direction);
        break;
    case BAR_BLOCK:
        freeRoute =checkTranslateBarRoute(orientation, xLeft, yTop, direction);
        break;
    }
    return freeRoute;
}

void translateBlock(int blockId, int & orientation, int & xLeft, int & yTop,
                    int fillingColor, int direction)
{
    switch(blockId)
    {
    case T_BLOCK  :
        translateTblock(orientation, xLeft, yTop, fillingColor, direction);
        break;
    case L_BLOCK  :
        translateLblock(orientation, xLeft, yTop, fillingColor, direction);
        break;
    case LR_BLOCK :
        translateLRblock(orientation, xLeft, yTop, fillingColor, direction);
        break;
    case BOX_BLOCK:
        translateBoxblock(orientation,xLeft,yTop,fillingColor,direction);
        break;
    case BAR_BLOCK:
        translateBarBlock(orientation, xLeft, yTop, fillingColor, direction);
        break;
    }
}

int checkRotateBarRoute(int orientation, int xLeft, int yTop)
{
    int maxNbCells, x, y, freeRoute;
    switch (orientation)
    {
    case HORIZONTAL:

        maxNbCells = 3;
        freeRoute = 1;
        x = xLeft + CELL_SIZE + CELL_SIZE/2;
        y = yTop  + CELL_SIZE + CELL_SIZE/2;
        do
            if (getpixel(x, y) == BOARD_COLOR )
            {
                x = x + CELL_SIZE;
                maxNbCells--;
            }
            else  freeRoute = 0;
        while (maxNbCells >=1 && freeRoute);

        if (freeRoute)
        {
            maxNbCells = 3;
            freeRoute = 1;
            x = xLeft + CELL_SIZE   + CELL_SIZE/2;
            y = yTop  + 2*CELL_SIZE + CELL_SIZE/2;
            do
                if (getpixel(x, y) == BOARD_COLOR )
                {
                    x = x + CELL_SIZE;
                    maxNbCells--;
                }
                else  freeRoute = 0;
            while (maxNbCells >=1 && freeRoute);
        }
        break;

    case VERTICAL:
        maxNbCells = 2;
        freeRoute = 1;
        x = xLeft - CELL_SIZE/2;
        y = yTop  + CELL_SIZE/2;
        do
            if (getpixel(x, y) == BOARD_COLOR )
            {
                y = y + CELL_SIZE;
                maxNbCells--;
            }
            else  freeRoute = 0;
        while (maxNbCells > 0 && freeRoute);

        if (freeRoute)
        {
            maxNbCells = 3;
            freeRoute  = 1;
            x = xLeft + CELL_SIZE + CELL_SIZE/2;
            y = yTop  + CELL_SIZE + CELL_SIZE/2;
            do
                if (getpixel(x, y) == BOARD_COLOR )
                {
                    y = y + CELL_SIZE;
                    maxNbCells--;
                }
                else  freeRoute = 0;
            while (maxNbCells > 0 && freeRoute);
        }

        if (freeRoute)
        {
            maxNbCells = 3;
            freeRoute = 1;
            x = xLeft + 2*CELL_SIZE + CELL_SIZE/2;
            y = yTop  + CELL_SIZE   + CELL_SIZE/2;
            do
                if (getpixel(x, y) == BOARD_COLOR )
                {
                    y = y + CELL_SIZE;
                    maxNbCells--;
                }
                else  freeRoute = 0;
            while (maxNbCells > 0 && freeRoute);
        }
        break;
    }
    return freeRoute;
}

int checkrotateLRroute(int orientation, int xLeft, int yTop)
{
    int freeRoute=1;
    switch(orientation)
    {
    case NORTH:
        xLeft=xLeft-CELL_SIZE/2;
        yTop=yTop+CELL_SIZE+CELL_SIZE/2;
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            xLeft=xLeft+2*CELL_SIZE;
            yTop=yTop-CELL_SIZE;
            if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                freeRoute=0;
        }
        else
            freeRoute=0;
        break;

    case WEST:
        xLeft=xLeft-CELL_SIZE/2;
        yTop=yTop+CELL_SIZE/2;
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            yTop=yTop+2*CELL_SIZE;
            xLeft=xLeft+CELL_SIZE;
            if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                freeRoute=0;
        }
        else
            freeRoute=0;
        break;

    case SOUTH:
        xLeft=xLeft+CELL_SIZE/2;
        yTop=yTop+2*CELL_SIZE+CELL_SIZE/2;
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            xLeft=xLeft+2*CELL_SIZE;
            yTop=yTop-CELL_SIZE;
            if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                freeRoute=0;
        }
        else
            freeRoute=0;
        break;

    case EAST:
        xLeft=xLeft+CELL_SIZE/2;
        yTop=yTop-CELL_SIZE/2;
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            yTop=yTop+2*CELL_SIZE;
            xLeft=xLeft+CELL_SIZE;
            if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                freeRoute=0;
        }
        else
            freeRoute=0;
        break;
    }
    return freeRoute;
}
int checkrotateTroute(int orientation, int xLeft, int yTop)
{
    int freeRoute=1;
    switch(orientation)
    {
    case NORTH:
        xLeft=xLeft+CELL_SIZE+CELL_SIZE/2;
        yTop=yTop-CELL_SIZE/2;
        if(getpixel(xLeft,yTop)!=BOARD_COLOR)
            freeRoute=0;
        break;

    case SOUTH:
        xLeft=xLeft+CELL_SIZE/2;
        yTop=yTop+2*CELL_SIZE+CELL_SIZE/2;
        if(getpixel(xLeft,yTop)!=BOARD_COLOR)
            freeRoute=0;
        break;

    case EAST:
        xLeft=xLeft-CELL_SIZE/2;
        yTop=yTop+CELL_SIZE+CELL_SIZE/2;
        if(getpixel(xLeft,yTop)!=BOARD_COLOR)
            freeRoute=0;
        break;

    case WEST:
        xLeft=xLeft+CELL_SIZE+CELL_SIZE/2;
        yTop=yTop+CELL_SIZE+CELL_SIZE/2;
        if(getpixel(xLeft,yTop)!=BOARD_COLOR)
            freeRoute=0;
        break;
    }
    return freeRoute;
}

int checkrotateLroute(int orientation, int xLeft, int yTop)
{
    int freeRoute=1;
    switch(orientation)
    {
    case NORTH:
        xLeft=xLeft-CELL_SIZE/2;
        yTop=yTop+CELL_SIZE+CELL_SIZE/2;
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            xLeft=xLeft+2*CELL_SIZE;
            if(getpixel(xLeft,yTop)==BOARD_COLOR)
            {
                yTop-=CELL_SIZE;
                if(getpixel(xLeft,yTop)!=0)
                    freeRoute=0;
            }
            else
                freeRoute=0;
        }
        else
            freeRoute=0;
        break;

    case SOUTH:
        xLeft=xLeft+CELL_SIZE/2;
        yTop=yTop+CELL_SIZE+CELL_SIZE/2;
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            yTop+=CELL_SIZE;
            if(getpixel(xLeft,yTop)==BOARD_COLOR)
            {
                xLeft=xLeft+2*CELL_SIZE;
                yTop-=CELL_SIZE;
                if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                    freeRoute=0;
            }
            else
                freeRoute=0;
        }
        else
            freeRoute=0;
        break;

    case EAST:
        xLeft=xLeft+CELL_SIZE+CELL_SIZE/2;
        yTop=yTop-CELL_SIZE/2;
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            yTop=yTop+2*CELL_SIZE;
            if(getpixel(xLeft,yTop)==BOARD_COLOR)
            {
                xLeft=xLeft+CELL_SIZE;
                if(getpixel(xLeft,yTop)!=0)
                    freeRoute=0;
            }
            else
                freeRoute=0;
        }
        else
            freeRoute=0;
        break;

    case WEST:
        xLeft=xLeft-CELL_SIZE/2;
        yTop=yTop+CELL_SIZE/2;
        if(getpixel(xLeft,yTop)==BOARD_COLOR)
        {
            xLeft-=CELL_SIZE;
            if(getpixel(xLeft,yTop)==BOARD_COLOR)
            {
                xLeft+=CELL_SIZE;
                yTop=yTop+2*CELL_SIZE;
                if(getpixel(xLeft,yTop)!=BOARD_COLOR)
                    freeRoute=0;
            }
            else
                freeRoute=0;
        }
        else
            freeRoute=0;
        break;
    }
    return freeRoute;
}

int rotateRouteFree(int blockId, int orientation, int xLeft, int yTop)
{
    int freeRoute;
    switch(blockId)
    {
    case T_BLOCK  :
        freeRoute=checkrotateTroute(orientation, xLeft, yTop);
        break;
    case L_BLOCK  :
        freeRoute=checkrotateLroute(orientation, xLeft, yTop);
        break;
    case LR_BLOCK :
        freeRoute=checkrotateLRroute(orientation, xLeft, yTop);
        break;
    case BOX_BLOCK:
        break;
    case BAR_BLOCK:
        freeRoute = checkRotateBarRoute(orientation, xLeft, yTop);
        break;
    }
    return freeRoute;
}

void rotateBarBlock(int & orientation, int & xLeft, int & yTop, int fillingColor)
{
    drawBarBlock(xLeft, yTop, orientation, BKGRD_COLOR);
    switch (orientation)
    {
    case HORIZONTAL:
        orientation = VERTICAL;
        xLeft = xLeft + CELL_SIZE;
        yTop  = yTop  - CELL_SIZE;
        break;

    case VERTICAL:
        orientation = HORIZONTAL;
        xLeft = xLeft - CELL_SIZE;
        yTop  = yTop  + CELL_SIZE;
        break;
    }
    drawBarBlock(xLeft, yTop, orientation, fillingColor);
}

void rotateLRblock(int &orientation, int &xLeft, int &yTop, int fillingcolor)
{
    drawLRblock(xLeft, yTop, orientation, BKGRD_COLOR);
    switch(orientation)
    {
    case NORTH:
        orientation=WEST;
        break;
    case WEST:
        orientation=SOUTH;
        xLeft=xLeft-CELL_SIZE;
        break;
    case SOUTH:
        orientation=EAST;
        xLeft=xLeft+CELL_SIZE;
        yTop=yTop+CELL_SIZE;
        break;
    case EAST:
        orientation=NORTH;
        yTop=yTop-CELL_SIZE;
        break;
    }
    drawLRblock(xLeft,yTop,orientation,fillingcolor);
}
void rotateTblock(int &orientation, int &xLeft, int &yTop, int fillingcolor)
{
    drawTblock(xLeft, yTop, orientation, BKGRD_COLOR);
    switch(orientation)
    {
    case NORTH:
        orientation=EAST;
        xLeft+=CELL_SIZE;
        yTop-=CELL_SIZE;
        break;
    case SOUTH:
        orientation=WEST;
        break;
    case EAST:
        orientation=SOUTH;
        break;
    case WEST:
        orientation=NORTH;
        xLeft-=CELL_SIZE;
        yTop+=CELL_SIZE;
        break;
    }
    drawTblock(xLeft,yTop,orientation,fillingcolor);
}

void rotateLblock(int &orientation, int &xLeft, int &yTop, int fillingcolor)
{
    drawLblock(xLeft, yTop, orientation, BKGRD_COLOR);
    switch(orientation)
    {
    case NORTH:
        orientation=WEST;
        xLeft+=CELL_SIZE;
        break;
    case SOUTH:
        orientation=EAST;
        yTop+=CELL_SIZE;
        break;
    case EAST:
        orientation=NORTH;
        xLeft+=CELL_SIZE;
        yTop-=CELL_SIZE;
        break;
    case WEST:
        orientation=SOUTH;
        xLeft-=CELL_SIZE;
        break;
    }
    drawLblock(xLeft,yTop,orientation,fillingcolor);
}

void rotateBlock(int blockId, int & orientation, int & xLeft, int & yTop,int fillingColor)
{
    switch(blockId)
    {
    case T_BLOCK  :
        rotateTblock(orientation, xLeft, yTop, fillingColor);
        break;
    case L_BLOCK  :
        rotateLblock(orientation, xLeft, yTop, fillingColor);
        break;
    case LR_BLOCK :
        rotateLRblock(orientation, xLeft, yTop, fillingColor);
        break;
    case BOX_BLOCK:
        break;
    case BAR_BLOCK:
        rotateBarBlock(orientation, xLeft, yTop, fillingColor);
        break;
    }
}

int checkColor(int row)
{
    int found=1, x=LEFTCORNER+CELL_SIZE+CELL_SIZE/2, y=TOPCORNER+(row*CELL_SIZE)-CELL_SIZE/2;
    for(int i=0; i<NB_COLS; i++)
    {
        if(getpixel(x,y)==BOARD_COLOR)
        {
            found=0;
            break;
        }
        x+=CELL_SIZE;
    }
    return found;
}

void changeRowColor(int row)
{
    int x=LEFTCORNER+CELL_SIZE+CELL_SIZE/2, y=TOPCORNER+(row*CELL_SIZE)-CELL_SIZE/2;
    for(int i=row; i>0; i--)
    {
        for(int j=0; j<NB_COLS; j++)
        {
            int Color=getpixel(x,(y-CELL_SIZE));
            setfillstyle(SOLID_FILL,Color);
            floodfill(x,y,GRID_COLOR);
            x+=CELL_SIZE;
        }
        x=LEFTCORNER+CELL_SIZE+CELL_SIZE/2;
        y-=CELL_SIZE;
    }
    lev+=1;
}

void changeColor()
{
    int found, nbrows=1;
    do
    {
        for(int i=NB_ROWS; i>0; i--)
        {
            found=checkColor(i);
            if(found)
                changeRowColor(i);
        }
        nbrows++;
    }
    while(nbrows<=NB_ROWS);

}
void handleUserInput(int blockId, int & orientation,int & xLeft, int & yTop, int fillingColor, int key)
{
    switch(key)
    {
    case LEFTARROW:
        if (translateRouteFree(blockId,orientation, xLeft, yTop, LEFT))
            translateBlock(blockId, orientation, xLeft, yTop, fillingColor, LEFT);
        displayInfo("left arrow key pressed...");
        break;
    case RIGHTARROW:
        if (translateRouteFree(blockId,orientation, xLeft, yTop, RIGHT))
            translateBlock(blockId, orientation, xLeft, yTop, fillingColor, RIGHT);
        displayInfo("right arrow key pressed...");
        break;
    case UPARROW:
        if (rotateRouteFree(blockId, orientation, xLeft, yTop))
            rotateBlock(blockId, orientation, xLeft, yTop, fillingColor);
        displayInfo("up arrow key pressed...");
        break;
    case DOWNARROW:
        if (translateRouteFree(blockId,orientation, xLeft, yTop, DOWN))
            translateBlock(blockId, orientation, xLeft, yTop, fillingColor, DOWN);
        displayInfo("down arrow key pressed...");
        break;
    }
}

void drawHighScore(int score)
{
    setcolor(YELLOW);
    settextstyle(BOLD_FONT,HORIZ_DIR,5);
    outtextxy(LEFTCORNER, (TOPCORNER+BOARDHEIGHT/2), "HIGH SCORE!");

}
void drawFinalScore(int score)
{
    char arr[50];
    sprintf(arr, "SCORE: %d",score);
    setcolor(WHITE);
    settextstyle(BOLD_FONT,HORIZ_DIR,6);
    outtextxy(LEFTCORNER-2*CELL_SIZE,(TOPCORNER+BOARDHEIGHT/2)+4*CELL_SIZE, arr);

}

void gameover(int score)
{
    bool HS=Scoreinput(score);
    setcolor(RED);
    setfillstyle(WIDE_DOT_FILL,RED);
    bar(LEFTCORNER-7*CELL_SIZE,TOPCORNER+4*CELL_SIZE,LEFTCORNER+BOARDWIDTH+7*CELL_SIZE,TOPCORNER+BOARDHEIGHT-2*CELL_SIZE);
    settextstyle(BOLD_FONT,HORIZ_DIR,8);
    outtextxy(LEFTCORNER-3*CELL_SIZE, TOPCORNER+BOARDHEIGHT/4, "GAME OVER!");
    drawFinalScore(score);
    if(HS)
        drawHighScore(score);
    delay(3000);
    setcolor(BLACK);
    setfillstyle(SOLID_FILL,BKGRD_COLOR);
    bar(LEFTCORNER-7*CELL_SIZE,TOPCORNER+4*CELL_SIZE,LEFTCORNER+BOARDWIDTH+7*CELL_SIZE,TOPCORNER+BOARDHEIGHT-2*CELL_SIZE);
    clearDisplayArea();
    drawBoard();
}


int gameOver()
{
    int xLeft, yTop, maxNbCells, gOver;
    yTop  = TOPCORNER + CELL_SIZE /2;
    xLeft = LEFTCORNER + 4*CELL_SIZE + CELL_SIZE/2;
    maxNbCells = 2;
    gOver = 0;
    while ( maxNbCells >= 1 && !gOver)
        if (getpixel(xLeft, yTop) == BOARD_COLOR )
        {
            xLeft = xLeft + CELL_SIZE;
            maxNbCells--;
        }
        else  gOver = 1;
    return gOver;
}

int escapeConfirmed()
{
    char ans;
    displayInfo("Are you sure you want to Exit [Y/N] ?>");
    do
        ans=toupper(char(getch()));
    while (ans != 'Y' && ans != 'N');
    if(ans == 'Y') return 1;
    else  return 0;
}

int freeFall(int blockId, int orientation, int & xLeft, int & yTop,int fillingColor)
{
    int fallBlocked = 0, level;
    if (translateRouteFree(blockId, orientation, xLeft, yTop, DOWN))
    {
        translateBlock(blockId, orientation, xLeft, yTop, fillingColor, DOWN);
        if(lev<5)
        {
            delay(LEVEL1_SPEED);
            level=1;
        }
        else if(lev>=5 && lev < 10)
        {
            delay(LEVEL2_SPEED);
            level=2;
        }
        else if(lev >= 10)
        {
            delay(LEVEL3_SPEED);
            level=3;
        }
        drawLevels(level);
    }
    else
    {
        fallBlocked = 1;
        changeColor();
    }
    return fallBlocked;
}

void playGame(int &score)
{
    score=0;
    int currentBlock, currentOrientation, key;
    int xLeftCurrent, yTopCurrent, fillingColor, fallBlocked;
    do
    {
        displayInfo("press ESC to abort the game");
        generateNewBlock(&currentBlock, &currentOrientation, &xLeftCurrent,&yTopCurrent, &fillingColor);
        do
        {
            do
            {
                fallBlocked = freeFall(currentBlock, currentOrientation, xLeftCurrent,yTopCurrent, fillingColor);
                if(fallBlocked)
                    score+=lev*10;
                drawScore(score);
            }
            while (!kbhit() && !fallBlocked);
            if (!fallBlocked)
            {
                key=getch();
                switch(key)
                {
                case ESC:
                    if (!escapeConfirmed()) key = 1;
                    break;
                case SPACE:
                    displayInfo("space key pressed...");
                    break;
                case '\0' :
                {
                    key = getch();
                    Clear();
                    handleUserInput(currentBlock, currentOrientation, xLeftCurrent,yTopCurrent, fillingColor, key);
                }
                }
            }
        }
        while (key != ESC && !fallBlocked);
    }
    while (key != ESC && !gameOver());

}

int main()
{
    srand(time(NULL));
    int key, score;
    initGraphEngine();
    do
    {
        drawBoard();
        // clearKeyBoard();
        Clear();
        clearDisplayArea();
        displayInfo("press ENTER key to play or ESC to Exit");
        key=getch();
        switch(key)
        {
        case ESC:
            if (!escapeConfirmed()) key = 1;
            break;
        case ENTER:
            playGame(score);
            gameover(score);
            displayInfo("Game Over!!! Press any key ---> ");
            lev=1;
            getch();
            break;
        }
    }
    while (key != ESC);
    displayInfo ("Come Again ... ");
    delay(1000);

    return 0;
}


