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


void Move(CHESS* chess, const POS src, const MOVEDATA desMoveData) // 기물을 움직이고 승패판정
{
	PIECETYPE catchEnemy = NONE;

	// 움직일 위치에 상대 말이 있는 경우
	if (chess->states[desMoveData.pos.y][desMoveData.pos.x].pieceType != NONE)
	{
		catchEnemy = chess->states[desMoveData.pos.y][desMoveData.pos.x].pieceType;
	}

	switch (chess->states[src.y][src.x].pieceType)
	{
	case KING: // King일 경우 캐슬링 확인
		if (desMoveData.isCastling != 0)
		{
			// Castling
			break;
		}
	default: {
		chess->states[desMoveData.pos.y][desMoveData.pos.x] = chess->states[src.y][src.x]; // 기물을 움직이고
		chess->states[src.y][src.x] = (STATEDATA){ .pieceType = NONE, .player = EMPTY_PLAYER, .moveCnt = 0 }; // 움직이기 전의 위치는 NONE으로
		}
	}

	// King의 위치를 찾는 부분------------
	POS kingPos = { 0, 0 };
	for (int y = 0; y < CHESS_SIZE; y++)
		for (int x = 0; x < CHESS_SIZE; x++)
			if (chess->states[y][x].pieceType == KING &&
				chess->states[y][x].player != chess->currentPlayer)
			{
				kingPos = (POS){ x,y };
				break;
			}
	//-----------------------------------


	int gameState = CalulateState(chess, kingPos); // 현재 어떤 상태인지, (Check, CheckMate, StaleMate)
	gotoxy(0, 31);
	if (gameState == 1)
	{
		//Check
		printf("  Check  ");
	}
	else if (gameState == 2)
	{
		if (CheckAround(chess, kingPos))
		{
			// CheckMate
			printf("CheckMate");
		}
	}
	else
	{
		if (CheckAround(chess, kingPos))
		{
			// StaleMate
			printf("StaleMate");
		}
		else
		{
			printf("         ");
		}
	}


	chess->currentPlayer = chess->currentPlayer == BLACK_PLAYER ? WHITE_PLAYER : BLACK_PLAYER;

}

// King이 공격을 받지 않음 == 0
// Check == 1
// 상대의 공격을 막을 수 없음 == 2
int CalulateState(CHESS* chess, const POS kingPos) // Check인지 CheckMate인지 StaleMate인지 판별함
{
	int kingColor = chess->states[kingPos.y][kingPos.x].player; // King의 색깔
	int dir[][2] = {
		{1,0}, {0,1}, {1,1}, {1, -1}
	}; // 방향을 나타냄, 상하 좌우 대각선

	BOOL existEnemy = FALSE;// kingPos를 공격할 수 있는 적이 있는지

	// 나이트를 제외한 말들이 King을 공격할 수 있는지
	for (int d = 0; d < 4; d++)
	{
		for (int s = -1; s <= 1; s += 2) // kingPos를 중심으로 방향을 나타냄
		{
			for (int i = 1; i < CHESS_SIZE - 1; i++) // kingPos를 중심으로 주변 1칸부터 쭉 탐색함
			{
				POS _checkPos = { kingPos.x + i * dir[d][0] * s, kingPos.y + i * dir[d][1] * s }; 

				// 맵 밖이면 break;
				if (!IsInMap(_checkPos))
					break;

				// King하고 같은 색깔의 기물이 있으면 다른 방향으로, break;
				if (chess->states[_checkPos.y][_checkPos.x].player == kingColor)
					break;


				// 상하나 좌우로 탐색할 때 
				if (d == 0 || d == 1)
				{
					// 상대편 ROOK이나 QUEEN 기물이 있으면
					if (chess->states[_checkPos.y][_checkPos.x].pieceType == ROOK ||
						chess->states[_checkPos.y][_checkPos.x].pieceType == QUEEN)
					{
						existEnemy = TRUE; // 상대편이 공격할 수 있다고 설정
						BOOL flag = FALSE; // 상대 기물을 공격하거나, 그 공격경로를 막을 수 있는지
						int color = kingColor == BLACK_PLAYER ? WHITE_PLAYER : BLACK_PLAYER;
						for (int j = i; j >= 1; j--)
						{
							POS _pos = { kingPos.x + j * dir[d][0] * s, kingPos.y + j * dir[d][1] * s };
							if (OtherCanCome(chess, _pos, color))
							{
								flag = TRUE;
								break;
							}
						}
						if (flag)
							return 1;
					}
				}
				// 대각선으로 탐색할 때
				else if (d == 2 || d == 3)
				{
					// 상대편 BISHOP이나 QUEEN 기물이 있으면
					if (chess->states[_checkPos.y][_checkPos.x].pieceType == BISHOP ||
						chess->states[_checkPos.y][_checkPos.x].pieceType == QUEEN)
					{
						existEnemy = TRUE; // 상대편이 공격할 수 있다고 설정
						BOOL flag = FALSE; // 상대 기물을 공격하거나, 그 공격경로를 막을 수 있는지
						int color = kingColor == BLACK_PLAYER ? WHITE_PLAYER : BLACK_PLAYER;
						for (int j = i; j >= 1; j--)
						{
							POS _pos = { kingPos.x + j * dir[d][0] * s, kingPos.y + j * dir[d][1] * s };
							if (OtherCanCome(chess, _pos, color))
							{
								flag = TRUE;
								break;
							}
						}
						if (flag)
							return 1;
					}
					// 상대편 PAWN이 있으면
					else if (chess->states[_checkPos.y][_checkPos.x].pieceType == PAWN)
					{
						// 탐색하고 있는 칸이 kingPos로부터 1칸떨어진 위치일 때
						if (i == 1)
						{
							// 검은색 플레이어일 때
							if (chess->currentPlayer == BLACK_PLAYER)
							{
								// 위쪽 대각선
								if ((d == 2 && s == -1) || (d == 3 && s == 1))
								{
									existEnemy = TRUE;
									int color = kingColor == BLACK_PLAYER ? WHITE_PLAYER : BLACK_PLAYER;
									if (OtherCanCome(chess, _checkPos, color))
										return 1;
								}
							}
							else
							{
								// 아래쪽 대각선
								if ((d == 2 && s == 1) || (d == 3 && s == -1))
								{
									existEnemy = TRUE;
									int color = kingColor == BLACK_PLAYER ? WHITE_PLAYER : BLACK_PLAYER;
									if (OtherCanCome(chess, _checkPos, color))
										return 1;
								}
							}
						}
					}
				}
				// 탐색위치가 빈 공간이 아니면, 더 이상 탐색할 수 없으므로 break;
				if (chess->states[_checkPos.y][_checkPos.x].player != NONE)
					break;
			}
		}
	}

	// 나이트가 King을 공격할 수 있는지
	int knightMoves[][2] = {
		{2, 1}, {1, 2}
	};

	for (int y = -1; y <= 1; y += 2)
	{
		for (int x = -1; x <= 1; x += 2)
		{
			for (int k = 0; k < 2; k++)
			{
				POS _checkPos = { kingPos.x + knightMoves[k][0] * x, kingPos.y + knightMoves[k][1] * y };
				if (!IsInMap(_checkPos))
					break;
				if (chess->states[_checkPos.y][_checkPos.x].pieceType == KNIGHT &&
					chess->states[_checkPos.y][_checkPos.x].player != kingColor)
				{
					existEnemy = TRUE;
					int color = kingColor == BLACK_PLAYER ? WHITE_PLAYER : BLACK_PLAYER;
					if (OtherCanCome(chess, _checkPos, color))
						return 1;
				}
			}
		}
	}
	if (existEnemy)
		return 2;
	return 0;
}

BOOL OtherCanCome(CHESS* chess, const POS pos, int playerColor)// playerColor와 다른 색의 말이 pos로 올 수 있는지 
{
	int posColor = chess->states[pos.y][pos.x].player; // pos 위치의 색깔
	int dir[][2] = {
		{1,0}, {0,1}, {1,1}, {1, -1}
	};

	// 나이트를 제외한 말들이 pos로 올 수 있는지
	for (int d = 0; d < 4; d++)
	{
		for (int s = -1; s <= 1; s += 2)
		{
			for (int i = 1; i < CHESS_SIZE - 1; i++)
			{
				POS _pos = { pos.x + i * dir[d][0] * s, pos.y + i * dir[d][1] * s };
				if (!IsInMap(_pos))
					break;

				// playerColor와 같은 색깔은 기물이면, break;
				if (chess->states[_pos.y][_pos.x].player == playerColor)
					break;

				if (d == 0 || d == 1)
				{
					if (chess->states[_pos.y][_pos.x].pieceType == ROOK ||
						chess->states[_pos.y][_pos.x].pieceType == QUEEN)
					{
						return TRUE;
					}
				}
				else if (d == 2 || d == 3)
				{
					if (chess->states[_pos.y][_pos.x].pieceType == BISHOP ||
						chess->states[_pos.y][_pos.x].pieceType == QUEEN)
					{
						return TRUE;
					}
					else if (chess->states[_pos.y][_pos.x].pieceType == PAWN)
					{
						if (posColor != EMPTY_PLAYER && i == 1)
						{
							if (chess->currentPlayer == BLACK_PLAYER)
							{
								// 위쪽 대각선
								if ((d == 2 && s == -1) || (d == 3 && s == 1))
									return TRUE;
							}
							else
							{
								// 아래쪽 대각선
								if ((d == 2 && s == 1) || (d == 3 && s == -1))
									return TRUE;
							}
						}
					}
				}

				if (chess->states[_pos.y][_pos.x].player != NONE)
					break;
			}
		}
	}

	// 나이트가 King을 공격할 수 있는지
	int knightMoves[][2] = {
		{2, 1}, {1, 2}
	};
	for (int y = -1; y <= 1; y += 2)
	{
		for (int x = -1; x <= 1; x += 2)
		{
			for (int k = 0; k < 2; k++)
			{
				POS _checkPos = { pos.x + knightMoves[k][0] * x, pos.y + knightMoves[k][1] * y };
				if (!IsInMap(_checkPos))
					break;

				if (chess->states[_checkPos.y][_checkPos.x].pieceType == KNIGHT &&
					chess->states[_checkPos.y][_checkPos.x].player != playerColor)
				{
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

BOOL CheckAround(CHESS* chess, const POS kingPos) // 움직이면 죽는지 확인
{
	BOOL aroundOnlySameColor = TRUE; // 주변 기물의 색깔이 다 kingPos의 색과 같은지
	BOOL aroundIsFull = TRUE; // 주변에 움직일 수 있는 위치가 있는지, 즉 pieceType이 NONE인 위치가 있는지

	for (int y = -1; y <= 1; y++)
	{
		for (int x = -1; x <= 1; x++)
		{
			// kingPos는 확인할 필요 없음
			if (x == 0 && y == 0)
				continue;
			POS _kingPos = { kingPos.x + x, kingPos.y + y };
			// 맵 밖도 확인할 필요 없음
			if (!IsInMap(_kingPos))
				continue;

			if (chess->states[_kingPos.y][_kingPos.x].pieceType != NONE)
			{
				// 다른 색의 기물이 있다면 aroundOnlySameColor를 False로 
				if (chess->states[_kingPos.y][_kingPos.x].player != chess->states[kingPos.y][kingPos.x].player)
					aroundOnlySameColor = FALSE;
				continue;
			}

			aroundIsFull = FALSE; // 위 if문을 토대로 주변에 빈 공간이 있음을 알 수 있음

			if (!OtherCanCome(chess, _kingPos, chess->states[kingPos.y][kingPos.x].player))
				return FALSE;
		}
	}
	// 주변에 빈 공간이 없더라고 다 같은 색이면, 움직일 수 없어도 상관 없음
	if (aroundOnlySameColor && aroundIsFull)
		return FALSE;
	return TRUE;

}

BOOL IsInMap(POS pos) // pos가 체스판 안에 있는지 확인
{
	if (pos.x < 0 || pos.x >= CHESS_SIZE || pos.y < 0 || pos.y >= CHESS_SIZE)
		return FALSE;
	return TRUE;
}