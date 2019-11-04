#pragma once

typedef enum _COLOR 
{ 
	Black = 0, Blue, Green, Cyan,
    Red, Magenta, Brown, Lgray, Dgray, 
	Lblue, Lgreen, Lcyan, Lred, Lmagenta, Yellow, White 
} COLOR;

/* 콘솔 관련 함수 */
void gotoxy(int y, int x);			 //gotoxy 함수
//void SetColor(WORD text, WORD back); //text와 background의 색상을 변경한다.

/* 게임 출력 함수 */
void InitBoard();					 //체스보드를 최초 출력한다.

void CursorView(char show);          //커서숨기기