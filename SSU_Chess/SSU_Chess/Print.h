#pragma once
#include "GameData.h"

#define GRID_SIZE 3
#define GAP 3

typedef enum _COLOR 
{ 
	Black = 0, Blue, Green, Cyan,
    Red, Magenta, Brown, Lgray, Dgray, 
	Lblue, Lgreen, Lcyan, Lred, Lmagenta, Yellow, White 
} COLOR;


/* 콘솔 관련 함수 */
void gotoxy(int y, int x);			 //gotoxy 함수
void SetColor(WORD text, WORD back); //text와 background의 색상을 변경한다.
void CursorView(char show);          //콘솔창에 커서 숨김 여부를 설정한다.

/* 게임 출력 함수 */
void PrintBoard(CHESS* chess, const MOVEDATA* md);
void PrintGrid(POS pos, STATEDATA sd, COLOR color, int canMove);
void PrintState(CHESS* chess);
void PrintRect(const POS pos, const int width, const int height);
void PrtMainMenu();					 //메인메뉴를 출력한다.