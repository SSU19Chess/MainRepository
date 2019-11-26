#include <stdio.h>
#include "Print.h"
#include "Game.h"
#include "GameData.h"

// �⹰�� ǥ���ϴ� ����
const char chessPiece[2][6] = {
	{'P', 'N', 'B', 'R', 'Q', 'K'},
	{'p', 'n', 'b', 'r', 'q', 'k'}
};

void SetColor(WORD text, WORD back) //text�� background�� ������ �����Ѵ�.
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text + back * 16);
}

void gotoxy(int y, int x) //gotoxy �Լ�
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void CursorView(char show)//Ŀ�������
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

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
void PrintBoard(CHESS* chess,const MOVEDATA* md) // ü���� ��ü�� ����ϴ� �Լ�
{
	int grid = 1;

	SetColor(Lgray, Black);

	// Y�� ��ȣ UI
	for (int y = 0; y < CHESS_SIZE; y++)
	{
		gotoxy( GAP + y * GRID_SIZE + 1, GAP);
		printf("%d", 8 - y);
		// ü���� �ܰ���
		for (int i = 0; i < 3; i++)
		{
			gotoxy( GAP + y * GRID_SIZE + i, GAP + 1);
			printf("��");
			gotoxy( GAP + y * GRID_SIZE + i, GAP + 3 + CHESS_SIZE * GRID_SIZE * 2);
			printf("��");

		}
	}

	// X�� ���ĺ� UI
	for (int x = 0; x < CHESS_SIZE; x++)
	{
		gotoxy( GAP / 2, (GAP + x * GRID_SIZE + 1) * 2);
		printf("%c", 'A' + x);

		// ü���� �ܰ���
		for (int i = 0; i < 3; i++)
		{
			gotoxy( GAP / 2 + 1, (GAP + x * GRID_SIZE + i) * 2);
			printf("��");
			gotoxy( GAP / 2 + 2 + CHESS_SIZE * GRID_SIZE, (GAP + x * GRID_SIZE + i) * 2);
			printf("��");
		}
	}
	// �𼭸� 
	gotoxy( GAP - 1, (GAP - 1) * 2);
	printf("��");
	gotoxy( GAP - 1, (GAP + CHESS_SIZE * GRID_SIZE) * 2);
	printf("��");
	gotoxy( GAP + CHESS_SIZE * GRID_SIZE, (GAP - 1) * 2);
	printf("��");
	gotoxy( GAP + CHESS_SIZE * GRID_SIZE, (GAP + CHESS_SIZE * GRID_SIZE) * 2);
	printf("��");



	for (int y = 0; y < CHESS_SIZE; y++)
	{
		for (int x = 0; x < CHESS_SIZE; x++)
		{
			COLOR color = grid == 1 ? White : Lgray;
			int canMove = IsInMoveData(md, (POS){ x,y });

			PrintGrid((POS){ x,y }, chess->states[y][x], color, canMove);

			grid = grid * -1;
		}
		grid = grid * -1;
	}
}

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
void PrintGrid(POS pos, STATEDATA sd, COLOR color, int canMove) // ü���ǿ��� ���� 1���� ����ϴ� �Լ�
{
	/*
	//������������
	wchar_t list[9] = {
		L'��', L'��', L'��',
		L'��', L'  ', L'��',
		L'��', L'��', L'��'
	};
	*/
	COLOR textColor = sd.player == WHITE_PLAYER ? Lmagenta : Dgray;
	for (int _y = 0; _y < GRID_SIZE; _y++)
	{
		for (int _x = 0; _x < GRID_SIZE; _x++)
		{
			gotoxy( (pos.y * GRID_SIZE + _y) + GAP, (pos.x * GRID_SIZE + _x + GAP) * 2);
			if (_x == 1 && _y == 1)
			{
				SetColor(textColor, canMove != -1? Lcyan : color);
				if (sd.player != EMPTY_PLAYER)
				{
					int sdC = sd.player == BLACK_PLAYER ? 1 : 0;
					printf("%c ", chessPiece[sdC][sd.pieceType - 1]);
				}
				else
				{
					SetColor(Lgray, canMove != -1 ? Lcyan : color);
					printf("  ");
				}
			}
			else
			{
				SetColor(Lgray, color);
				printf("  ");
				//wprintf(L"%c", list[_x + _y * GRID_SIZE]);
			}
		}
	}
}

/**
	@ �Լ� �̸�: PrintState
	@ �Լ� ����: ���� � �÷��̾ ���� �ξ���ϴ� ��������, �� �÷��̾ ���� �⹰�� ������ �ִ����� ǥ���Ѵ�.
	@ �Ķ���� �̸� ���� (chess)
	@ �Ķ���� ����
		@ chess: ü�� ���ӿ� ���� ������ ������ �ִ� ����ü
	@ ���� �Լ���: SetColor, gotoxy, PrintRect
**/
void PrintState(CHESS* chess) // ü���� ���� ���ӿ� ���� �߰����� ������ ǥ���ϴ� �Լ�
{
	// ���� �÷��̾ � ������ ǥ��-------------------------------------
	PrintRect((POS){ 1, -1 }, 5, 1);
	// ���� ǥ��
	gotoxy( GAP, (GAP + CHESS_SIZE * GRID_SIZE + 2) * 2);
	printf("%s", chess->currentPlayer == BLACK_PLAYER ? "BLACK" : "WHITE");


	// �� �÷��̾ ���� �⹰ ǥ��-------------------------------------

	PrintRect((POS) { 1, 3 }, 20, 2);
	SetColor(Lmagenta, Black);
	for (int i = 0; i < 20; i++)
	{
		if (chess->printInfo.diedPiece[1][i] == NONE)
			break;

		gotoxy(GAP + 4 + (i / 10), (GAP + CHESS_SIZE * GRID_SIZE + 2 + i) * 2);
		printf("%c ", chessPiece[0][chess->printInfo.diedPiece[1][i] - 1]);

	}


	PrintRect((POS) { 1, 17 }, 20, 2);
	SetColor(Dgray, Black);
	for (int i = 0; i < 20; i++)
	{
		if (chess->printInfo.diedPiece[0][i] == NONE)
			break;

		gotoxy(GAP + 18 + (i / 10), (GAP + CHESS_SIZE * GRID_SIZE + 2 + i) * 2);
		printf("%c ", chessPiece[1][chess->printInfo.diedPiece[0][i] - 1]);

	}
	// Check ��� ǥ��

	PrintRect((POS) { 5, -1}, 12, 1);
	gotoxy(GAP, (GAP + CHESS_SIZE * GRID_SIZE + 6 ) * 2);
	switch (chess->printInfo.gameState)
	{
	case 1:
		printf("   Check");
		break;
	case 2:
		printf(" CheckMate");
		break;
	case 3:
		printf(" StaleMate");
	default:
		printf("           ");
	}
}

/**
	@ �Լ� �̸�: PrintResult
	@ �Լ� ����: �¸� �Ǵ� ���ºθ� ǥ���ϴ� �Լ��̴�.
	@ �Ķ���� �̸� ���� (winPlayerColor)
	@ �Ķ���� ����
		@ winPlayerColor: �¸��� �÷��̾��� ������ ǥ���Ѵ�, 0�� ���º��̴�.
	@ ���� �Լ���: SetColor, gotoxy, PrintRect
**/
void PrintResult(int winPlayerColor) // BLACK_PLAYER, WHITE_PLAYER, ( 0 == ���º�)
{
	PrintRect((POS) { 1, 8 }, 20, 6);
	if (winPlayerColor != 0)
	{
		gotoxy(GAP + 11, (GAP + CHESS_SIZE * GRID_SIZE + 5) * 2 + 1);
		printf("%s", winPlayerColor == BLACK_PLAYER ? "BLACK" : "WHITE");
		gotoxy(GAP + 12, (GAP + CHESS_SIZE * GRID_SIZE + 5) * 2 + 1);
		printf("[WIN]");
	}
	else
	{
		gotoxy(GAP + 12, (GAP + CHESS_SIZE * GRID_SIZE + 5) * 2 + 1);
		printf("[DRAW]");
	}
}

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
void PrintRect(const POS pos,const int _width, const int height)
{
	int i;
	int mod = _width % 2;
	int width = _width / 2 + mod;

	SetColor(Lgray, Black);
	// ��, �Ʒ� ��
	for (i = 1; i <= width; i++)
	{
		gotoxy(GAP + pos.y, (GAP + CHESS_SIZE * GRID_SIZE + pos.x + i) * 2);
		printf("�� ");

		gotoxy(GAP + pos.y + height + 1, (GAP + CHESS_SIZE * GRID_SIZE + pos.x + i) * 2);
		printf("�� ");
	}
	// �𼭸� �׸���
	gotoxy(GAP + pos.y, (GAP + CHESS_SIZE * GRID_SIZE + pos.x) * 2);
	printf("�� ");
	gotoxy(GAP + pos.y, (GAP + CHESS_SIZE * GRID_SIZE + pos.x + i) * 2 - mod);
	printf("�� ");
	gotoxy(GAP + pos.y + height + 1, (GAP + CHESS_SIZE * GRID_SIZE + pos.x) * 2);
	printf("�� ");
	gotoxy(GAP + pos.y + height + 1, (GAP + CHESS_SIZE * GRID_SIZE + pos.x + i) * 2 - mod);
	printf("�� ");

	// ����
	for (i = 1; i <= height; i++)
	{
		gotoxy(GAP + pos.y + i, (GAP + CHESS_SIZE * GRID_SIZE + pos.x) * 2);
		printf("�� ");
		gotoxy(GAP + pos.y + i, (GAP + CHESS_SIZE * GRID_SIZE + pos.x + width + 1) * 2 - mod);
		printf("�� ");
	}
}

void PrtMainMenu()
{
	printf("���ߡߡߡߡ��ߡߡߡߡ��ߡ����ߡ�����������������\n");
	printf("���ߡ��������ߡ��������ߡ����ߡ������ߡߡ�������\n");
	printf("���ߡߡߡߡ��ߡߡߡߡ��ߡ����ߡ����ߡ����ߡ�����\n");
	printf("���������ߡ��������ߡ��ߡ����ߡ������������ߡߡ�\n");
	printf("���ߡߡߡߡ��ߡߡߡߡ����ߡߡ�������������������\n\n");
	printf("�����ߡߡߡ��ߡ����ߡ��ߡߡߡߡ��ߡߡߡߡ��ߡߡߡ�\n");
	printf("���ߡ��������ߡ����ߡ��ߡ��������ߡ��������ߡ�����\n");
	printf("���ߡ��������ߡߡߡߡ��ߡߡߡߡ��ߡߡߡߡ��ߡߡߡ�\n");
	printf("���ߡ��������ߡ����ߡ��ߡ��������������ߡ���������\n");
	printf("�����ߡߡߡ��ߡ����ߡ��ߡߡߡߡ��ߡߡߡߡ��ߡߡߡ�\n\n\n");
	printf("�����������������ӽ��ۡ���������������������������\n");
	printf("����������������������\n");


	gotoxy(13, 10);
	printf("��");
}