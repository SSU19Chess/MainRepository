#pragma once

enum color
{
	Black = 0, Blue, Green, Cyan, Red, Magenta, Brown, Lgray, Dgray, Lblue, Lgreen,
	Lcyan, Lred, Lmagenta, Yellow, White
};


void GotoXY(int y, int x);			 //gotoxy �Լ�
void SetColor(WORD text, WORD back); //text�� background�� ������ �����Ѵ�.
void InitBoard();					 //ü�����带 ���� ����Ѵ�.