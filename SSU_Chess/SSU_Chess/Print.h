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

/**
	@ �Լ� �̸�: gotoxy
	@ �Լ� ����: ��� ������Ʈ�� Ư�� ��ǥ�� �̵��Ѵ�.
	@ �Ķ���� �̸� ���� (y, x)
	@ �Ķ���� ����
		@ y, x : �̵��� ��ġ��ǥ
**/
void gotoxy(int y, int x);			 //gotoxy �Լ�

/**
	@ �Լ� �̸�: SetColor
	@ �Լ� ����: text�� background�� ������ �����Ѵ�.
	@ �Ķ���� �̸� ���� (text, back)
	@ �Ķ���� ����
		@ text : text ����
		@ back : background ����
**/
void SetColor(WORD text, WORD back); //text�� background�� ������ �����Ѵ�.

/**
	@ �Լ� �̸�: CursorView
	@ �Լ� ����: Ŀ���� ǥ�� ���θ� ������Ʈ �Ѵ�.
	@ �Ķ���� �̸� ���� (show)
	@ �Ķ���� ����
		@ show : Ŀ��ǥ�� ���� ��
**/
void CursorView(char show);          //�ܼ�â�� Ŀ�� ���� ���θ� �����Ѵ�.

/* ���� ��� �Լ� */

/**
	@ �Լ� �̸�: PrintBoard
	@ �Լ� ����: �⺻���� ü����, ��ũ�� ������ ȭ�鿡 �����ָ�
				 chess ����ü�� ������� �⹰�� ǥ���ϰ� md ����ü�� ������� �⹰�� ������ �� �ִ� ��ġ�� ǥ���Ѵ�.
	@ �Ķ���� �̸� ���� (chess, md)
	@ �Ķ���� ����
		@ chess: ü�� ���ӿ� ���� ������ ������ �ִ� ����ü
		@ md: Ư�� �⹰�� ������ �� �ִ� ��ġ�� ���� ������ ���� ����ü �迭
	@ ���� �Լ���: SetColor, gotoxy, PrintGrid
**/
void PrintBoard(CHESS* chess, const MOVEDATA* md);

/**
	@ �Լ� �̸�: PrintGrid
	@ �Լ� ����: �⺻���� ü���ǿ��� ���� 1ĭ�� �׸��� �Լ��̴�.
	@ �Ķ���� �̸� ���� (pos, sd, color, canMove)
	@ �Ķ���� ����
		@ pos: ������ ��ġ�� x, y ��� 1 ~ CHESS_SIZE ���� �ȿ� �ִ�.
		@ sd: pos ��ġ�� �ִ� ü������ ������ �÷��̾��� ����, �⹰�� ���� ���� ������ �ִ�.
		@ color: ������ ������ ������ �ִ�. ��� or ������
		@ canMove: ���� ��ġ�� ���ڰ� �⹰�� ������ �� �ִ����� �˷��ش�. canMove�� -1�� �ƴϸ� ���� �߾��� �ٸ� ������ ǥ�õȴ�.
	@ ���� �Լ���: SetColor, gotoxy
**/
void PrintGrid(POS pos, STATEDATA sd, COLOR color, int canMove);

/**
	@ �Լ� �̸�: PrintState
	@ �Լ� ����: ���� � �÷��̾ ���� �ξ���ϴ� ��������, �� �÷��̾ ���� �⹰�� ������ �ִ����� ǥ���Ѵ�.
	@ �Ķ���� �̸� ���� (chess)
	@ �Ķ���� ����
		@ chess: ü�� ���ӿ� ���� ������ ������ �ִ� ����ü
	@ ���� �Լ���: SetColor, gotoxy, PrintRect
**/
void PrintState(CHESS* chess);

/**
	@ �Լ� �̸�: PrintResult
	@ �Լ� ����: �¸� �Ǵ� ���ºθ� ǥ���ϴ� �Լ��̴�.
	@ �Ķ���� �̸� ���� (winPlayerColor)
	@ �Ķ���� ����
		@ winPlayerColor: �¸��� �÷��̾��� ������ ǥ���Ѵ�, 0�� ���º��̴�.
	@ ���� �Լ���: SetColor, gotoxy, PrintRect
**/
void PrintResult(int winPlayerColor);

/**
	@ �Լ� �̸�: PrintRect
	@ �Լ� ����: ȭ�鿡 Ư�� ��ȣ�� ����Ͽ� ���� �� �簢���� ǥ���ϴ� �Լ��̴�.
	@ �Ķ���� �̸� ���� (pos, width, height )
	@ �Ķ���� ����
		@ pos: �簢���� ���� ����� ��ġ��, x ��ǥ�� ü���� ���� ����� 0���� �����Ѵ�.
		@ width: �簢�� ������ �ʺ�
		@ height: �簢�� ������ ����
	@ ���� �Լ���: SetColor, gotoxy
**/
void PrintRect(const POS pos, const int width, const int height);

/**
	@ �Լ� �̸�: PrtMainMenu
	@ �Լ� ����: ������ ���θ޴��� ����Ѵ�.
	@ ���� �Լ���: gotoxy
**/
void PrtMainMenu();

/**
	@ �Լ� �̸�: PrintPromoSel
	@ �Լ� ����: ���θ���� �� �⹰�� ������ �������̽��� ����Ѵ�.
	@ �Ķ���� �̸� ���� (curColor)
	@ �Ķ���� ����
		@ curColor : ���θ�� �� Pawn�� ������
	@ ���� �Լ��� : SetColor
**/
void PrintPromoSel(int curColor);