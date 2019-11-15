#include "Game.h"
#include "Print.h"
#include "GameData.h"
#include <stdio.h>
#include <Windows.h>
#include <conio.h>

int MainMenu()
{
	CursorView(0);

	int ypos = 13;

	PrtMainMenu();

	while (1)
	{
		gotoxy(ypos, 10);

		int input = _getch();
		if (input == 224)
		{
			input = _getch();
			switch (input)
			{
			case UP:
				ypos = 13;
				printf("  ");
				break;
			case DOWN:
				ypos = 14;
				printf("  ");
				break;
			}
			gotoxy(ypos, 10);
			printf("��");
		}
		else if (input == ENTER)
		{
			system("cls");
			return ypos == 14 ? -1 : 0;
		}
	}
}

//�Ű������� ���޵� ü������ ������� �̵������� ��ġ ���͸� ��ȯ
MOVEDATA* GetMoveData(CHESS* ch, const POS pos)
{
	MOVEDATA* ret;
	const STATEDATA curState = ch->states[pos.y][pos.x];
	const int curColor = curState.player;			 //���� �÷��̾��� color
	const int oppoColor = curColor * -1;			 //��� �÷��̾��� color

	int cnt = 0;									 //ret�� ���� ��, �̵������� ��ġ�� ����
	ret = (MOVEDATA*)malloc(sizeof(MOVEDATA) * cnt); //���̰� 0�� �� ���� �Ҵ�


	/* Ư�� PIECETYPE�� MOVEDATA */
	const int knightMovDataY[6] = { 1, 1, -1, -1, 2, -2 }; //KNIGHT�� �̵� ������
	const int knightMovDataX[6] = { 2, -2, 2, -2, 1, -1 };

	const int bishopMovDataY[4] = { 1, 1, -1, -1 };		  //BISHOP�� �̵� ������
	const int bishopMovDataX[4] = { -1, 1, -1, 1 };

	const int rookMovDataY[4] = {1, -1, 0, 0};			//ROOK�� �̵� ������
	const int rookMovDataX[4] = {0, 0, 1, -1};


	switch (curState.pieceType)
	{
	case PAWN: //pos�� ��ġ�� �⹰�� PAWN�� ���

		if (ch->states[pos.y + curColor][pos.x - 1].pieceType == oppoColor) //�밢���� �� �⹰�� �ִ��� �Ǵ�
		{
			if (IsAvailableToMov(ch, pos.y - 1, pos.x - 1, curColor))
			{
				ret = (MOVEDATA*)realloc(ret, sizeof(MOVEDATA) * (++cnt));
				ret[cnt - 1].pos.y = pos.y + curColor;
				ret[cnt - 1].pos.x = pos.x - 1;
			}
		}
		if (ch->states[pos.y + curColor][pos.x + 1].pieceType == oppoColor)
		{ 
			if (IsAvailableToMov(ch, pos.y - 1, pos.x + 1, curColor))
			{
				ret = (MOVEDATA*)realloc(ret, sizeof(MOVEDATA) * (++cnt));
				ret[cnt - 1].pos.y = pos.y + curColor;
				ret[cnt - 1].pos.x = pos.x + 1;
			}
		}

		if (curState.moveCnt == 0) //Pawn�� �ѹ��� �������� �ʾ��� ���
		{
			for (int i = 0; i <= 1; i++)
			{
				POS nextPos = { pos.y + (curColor * (i + 1)), pos.x };

				if (!IsEmpty(ch, nextPos.y, nextPos.x)) break;
				if (!IsAvailableToMov(ch, nextPos.y, nextPos.x, curColor)) break;

				ret = (MOVEDATA*)realloc(ret, sizeof(MOVEDATA) * (++cnt));

				ret[cnt - 1].isCastling = FALSE;
				ret[cnt-1].pos.y = nextPos.y;
				ret[cnt-1].pos.x = nextPos.x;
			}
		}
		else //Pawn�� �ѹ� �̻� �������� ���
		{
			if (!IsEmpty(ch, pos.y + curColor, pos.x)) break;

			ret = (MOVEDATA*)realloc(ret, sizeof(MOVEDATA) * (++cnt));
			ret[cnt - 1].pos.y = pos.y + curColor;
			ret[cnt - 1].pos.x = pos.x;
		}
		break;

	case KNIGHT: //pos�� ��ġ�� �⹰�� KNIGHT�� ���
		
		for (int i = 0; i < 6; i++)
		{
			POS nextPos = { pos.y + knightMovDataY[i], pos.x + knightMovDataX[i] };
			if (IsAvailableToMov(ch, nextPos.y, nextPos.x, curColor))
			{
				ret = (MOVEDATA*)realloc(ret, sizeof(MOVEDATA) * (++cnt));
				ret[cnt - 1].pos.y = nextPos.y;
				ret[cnt - 1].pos.x = nextPos.x;
			}
		}
		break;

	case BISHOP: //pos�� ��ġ�� �⹰�� BISHOP�� ���

		for (int i = 0; i < 4; i++)
		{
			int plus = 1;
			POS nextPos = { pos.y + plus * bishopMovDataY[i], pos.x + plus * bishopMovDataX[i] };

			while (IsAvailableToMov(ch, nextPos.y, nextPos.x, curColor))
			{
				ret = (MOVEDATA*)realloc(ret, sizeof(MOVEDATA) * (++cnt));
				ret[cnt - 1].pos.y = nextPos.y;
				ret[cnt - 1].pos.x = nextPos.x;

				nextPos.y = pos.y + plus * bishopMovDataY[i];
				nextPos.x = pos.x + plus * bishopMovDataX[i];
				plus++;
			}
		}
		break;

	case ROOK: //pos�� ��ġ�� �⹰�� ROOK�� ���
	


		for (int i = 0; i < 4; i++)
		{
			int plus = 1;
			POS nextPos = { pos.y + plus * rookMovDataY[i], pos.x + plus * bishopMovDataX[i] };

			while (IsAvailableToMov(ch, nextPos.y, nextPos.x, curColor))
			{
				ret = (MOVEDATA*)realloc(ret, sizeof(MOVEDATA) * (++cnt));
				ret[cnt - 1].pos.y = nextPos.y;
				ret[cnt - 1].pos.x = nextPos.x;

				nextPos.y = pos.y + plus * rookMovDataY[i];
				nextPos.x = pos.x + plus * rookMovDataX[i];
				plus++;
			}
		}
		break;

	case QUEEN:
		break;
	case KING:
		break;
	}

	return ret;
}

BOOL IsEmpty(CHESS* ch, int y, int x) //�ش� ĭ�� ��������� TRUE
{
	if (ch->states[y][x].pieceType == NONE)
		return TRUE;
	else
		return FALSE;
}

BOOL IsAvailableToMov(CHESS* ch, int y, int x, int curColor) //�ش� ���� y, x�� �̵��� �� �ִ��� ���� ��ȯ
{
	if (ch->states[y][x].player == curColor) //�ش� ĭ�� ������ �⹰�� ������ FALSE
		return FALSE;

	if (y < 0 || y >= 8 || x < 0 || x >= 8) //���� ���� ���̸� FALSE
		return FALSE;
	
	return TRUE;
}