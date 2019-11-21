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


/* �ܼ� ���� �Լ� */
void gotoxy(int y, int x);			 //gotoxy �Լ�
void SetColor(WORD text, WORD back); //text�� background�� ������ �����Ѵ�.
void CursorView(char show);          //�ܼ�â�� Ŀ�� ���� ���θ� �����Ѵ�.

/* ���� ��� �Լ� */
void PrintBoard(CHESS* chess, const MOVEDATA* md);
void PrintGrid(POS pos, STATEDATA sd, COLOR color, int canMove);
void PrintState(CHESS* chess);
void PrintRect(const POS pos, const int width, const int height);
void PrtMainMenu();					 //���θ޴��� ����Ѵ�.