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
			printf("☞");
		}
		else if (input == ENTER)
		{
			system("cls");
			return ypos == 14 ? -1 : 0;
		}
	}
}

//매개변수로 전달된 체스판을 기반으로 이동가능한 위치 벡터를 반환
MOVEDATA* GetMoveData(CHESS* ch, const POS pos)
{
	MOVEDATA* ret;
	const STATEDATA curState = ch->states[pos.y][pos.x];
	const int curColor = curState.player;			 //현재 플레이어의 color
	const int oppoColor = curColor * -1;			 //상대 플레이어의 color

	int cnt = 0;									 //ret의 길이 즉, 이동가능한 위치의 개수
	ret = (MOVEDATA*)malloc(sizeof(MOVEDATA) * cnt); //길이가 0인 빈 공간 할당


	/* 특정 PIECETYPE의 MOVEDATA */
	const int knightMovDataY[6] = { 1, 1, -1, -1, 2, -2 }; //KNIGHT의 이동 데이터
	const int knightMovDataX[6] = { 2, -2, 2, -2, 1, -1 };

	const int bishopMovDataY[4] = { 1, 1, -1, -1 };		  //BISHOP의 이동 데이터
	const int bishopMovDataX[4] = { -1, 1, -1, 1 };

	const int rookMovDataY[4] = {1, -1, 0, 0};			//ROOK의 이동 데이터
	const int rookMovDataX[4] = {0, 0, 1, -1};


	switch (curState.pieceType)
	{
	case PAWN: //pos에 위치한 기물이 PAWN일 경우

		if (ch->states[pos.y + curColor][pos.x - 1].pieceType == oppoColor) //대각선에 적 기물이 있는지 판단
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

		if (curState.moveCnt == 0) //Pawn이 한번도 움직이지 않았을 경우
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
		else //Pawn이 한번 이상 움직였을 경우
		{
			if (!IsEmpty(ch, pos.y + curColor, pos.x)) break;

			ret = (MOVEDATA*)realloc(ret, sizeof(MOVEDATA) * (++cnt));
			ret[cnt - 1].pos.y = pos.y + curColor;
			ret[cnt - 1].pos.x = pos.x;
		}
		break;

	case KNIGHT: //pos에 위치한 기물이 KNIGHT일 경우
		
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

	case BISHOP: //pos에 위치한 기물이 BISHOP일 경우

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

	case ROOK: //pos에 위치한 기물이 ROOK인 경우
	


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

BOOL IsEmpty(CHESS* ch, int y, int x) //해당 칸이 비어있으면 TRUE
{
	if (ch->states[y][x].pieceType == NONE)
		return TRUE;
	else
		return FALSE;
}

BOOL IsAvailableToMov(CHESS* ch, int y, int x, int curColor) //해당 말이 y, x로 이동할 수 있는지 여부 반환
{
	if (ch->states[y][x].player == curColor) //해당 칸에 같은편 기물이 있으면 FALSE
		return FALSE;

	if (y < 0 || y >= 8 || x < 0 || x >= 8) //보드 영역 밖이면 FALSE
		return FALSE;
	
	return TRUE;
}