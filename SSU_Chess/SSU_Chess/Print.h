#pragma once

typedef enum _COLOR 
{ 
	Black = 0, Blue, Green, Cyan,
    Red, Magenta, Brown, Lgray, Dgray, 
	Lblue, Lgreen, Lcyan, Lred, Lmagenta, Yellow, White 
} COLOR;

/* �ܼ� ���� �Լ� */
void gotoxy(int y, int x);			 //gotoxy �Լ�
//void SetColor(WORD text, WORD back); //text�� background�� ������ �����Ѵ�.

/* ���� ��� �Լ� */
void InitBoard();					 //ü�����带 ���� ����Ѵ�.

void CursorView(char show);          //Ŀ�������