#pragma once

enum color
{
	Black = 0, Blue, Green, Cyan, Red, Magenta, Brown, Lgray, Dgray, Lblue, Lgreen,
	Lcyan, Lred, Lmagenta, Yellow, White
};


void GotoXY(int y, int x);			 //gotoxy 함수
void SetColor(WORD text, WORD back); //text와 background의 색상을 변경한다.
void InitBoard();					 //체스보드를 최초 출력한다.