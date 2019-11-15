#include <stdio.h>
#include <conio.h>

#include "Game.h"
#include "Print.h"
#include "GameData.h"

// 초기 게임 판 구성
const PIECETYPE initPieces[CHESS_SIZE][CHESS_SIZE] = {
	{ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK},
	{PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN},
	{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
	{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
	{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
	{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
	{PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN},
	{ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK}
};

int MainMenu()
{
	// 초기 세팅 -------------------------------
	//CursorView(0);

	// 변수 선언 -------------------------------
	int ypos = 13;


	// 시작화면---------------------------------
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

int InGame()
{
	// 변수 선언 -------------------------------
	CHESS* chess = Init();			// 체스 게임 정보
	POS currentPos = { 0,0 };		// 체스판에서의 현재 위치
	POS lastSelectedPos = { -1, -1 };	// 선택한 기물의 위치, 선택한 기물이 없다면 {-1, -1}
	MOVEDATA* moveData = NULL;		// 선택한 기물이 움직일 수 있는 위치 정보들

	// 인 게임화면------------------------------
	while (1)
	{
		PrintBoard(chess, moveData);
		PrintState(chess);

		Input(chess, &lastSelectedPos, &currentPos, &moveData);
	}
}

void Input(CHESS* chess, POS* lastSelectedPos, POS* currentPos, MOVEDATA** moveData)
{
	BOOL r = TRUE;
	char c;
	while (r)
	{
		gotoxy(currentPos->y * GRID_SIZE + 1 + GAP, (currentPos->x * GRID_SIZE + 1 + GAP) * 2);
		c = _getch();
		switch (c)
		{
		case RIGHT:      //방향키 우측 시 커서 x + 1
		{
			currentPos->x++;

			if (currentPos->x >= CHESS_SIZE)   //오목판 크기를 벗어나는 값일 경우 x - 1
			{
				currentPos->x--;
			}

			break;
		}

		case LEFT:      //방향키 좌측 시 커서 x - 1
		{
			currentPos->x--;

			if (currentPos->x < 0)      //오목판 크기를 벗어나는 값일 경우 x + 1
			{
				currentPos->x++;
			}

			break;
		}

		case UP:      //방향키 위측 시 커서 y - 1
		{
			currentPos->y--;

			if (currentPos->y < 0)      //오목판 크기를 벗어나는 값일 경우 y + 1
				currentPos->y++;

			break;
		}

		case DOWN:      //방향키 아래측 시 커서 y + 1
		{
			currentPos->y++;

			if (currentPos->y >= CHESS_SIZE)   //오목판 크기를 벗어나는 값일 경우 y - 1
				currentPos->y--;

			break;
		}

		case ENTER:      //엔터키 입력 시
		{
			if (lastSelectedPos->x == -1 || lastSelectedPos->y == -1) // 아무것도 선택이 안된 상태일 때
			{
				if (chess->states[currentPos->y][currentPos->x].pieceType != NONE)
				{
					if (chess->states[currentPos->y][currentPos->x].player == chess->currentPlayer)
					{
						memcpy(lastSelectedPos, currentPos, sizeof(POS));
						*moveData = GetMoveData(chess, *currentPos);
					}
				}
			}
			else
			{
				int index = IsInMoveData(moveData, *currentPos);
				if (index != -1)
				{
					Move(chess, *lastSelectedPos, *(*moveData + index));
					*lastSelectedPos = (POS){ -1,-1 };
				}
			}
			r = FALSE;      //r의 값에 0을 대입 (무한루프 탈출)
			break;
		}
		}
	}
}

// 체스판을 초기화하는 함수
CHESS* Init()
{
	CHESS* chess = (CHESS*)malloc(sizeof(CHESS));
	if (chess == NULL)
		return NULL;

	for (int y = 0; y < CHESS_SIZE; y++)
	{
		for (int x = 0; x < CHESS_SIZE; x++)
		{
			int color = EMPTY_PLAYER;
			if (y <= 1)
				color = BLACK_PLAYER;
			else if (y >= 6)
				color = WHITE_PLAYER;

			chess->states[y][x] = (STATEDATA){ initPieces[y][x], color, 0 };
		}
	}

	chess->kingSideCastling[0] = TRUE;
	chess->queenSideCastling[0] = TRUE;
	chess->kingSideCastling[1] = TRUE;
	chess->queenSideCastling[1] = TRUE;

	chess->currentPlayer = WHITE_PLAYER;

	return chess;
}

//해당 말이 y, x로 이동할 수 있는지 여부 반환
BOOL IsAvailableToMov(CHESS* ch, int y, int x, int curColor) 
{
	if (ch->states[y][x].player == curColor) //해당 칸에 같은편 기물이 있으면 FALSE
		return FALSE;

	if(!IsInMap((POS){x, y}))
		return FALSE;
	
	return TRUE;
}

//해당 칸이 비어있으면 TRUE
BOOL IsEmpty(CHESS* ch, int y, int x)
{
	if (!IsInMap((POS) { x, y }))
		return FALSE;

	if (ch->states[y][x].pieceType == NONE)
		return TRUE;
	else
		return FALSE;
}

// playerColor와 다른 색의 말이 pos로 올 수 있는지 
BOOL OtherCanCome(CHESS* chess, const POS pos, int playerColor)
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

// 움직이면 죽는지 확인
BOOL CheckAround(CHESS* chess, const POS kingPos) 
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

// pos가 체스판 안에 있는지 확인
BOOL IsInMap(POS pos) 
{
	if (pos.x < 0 || pos.x >= CHESS_SIZE || pos.y < 0 || pos.y >= CHESS_SIZE)
		return FALSE;
	return TRUE;
}

// pos가 MoveData 목록 안에 있는지
int IsInMoveData(MOVEDATA* md, const POS pos)
{
	if( md == NULL)
		return -1;

	size_t size = _msize(md) / sizeof(MOVEDATA);
	for (int i = 0; i < size; i++)
	{
		if (md[i].pos.x == pos.x && md[i].pos.y == pos.y)
			return i;
	}
	return -1;
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
	const int knightMovDataY[8] = { 2, 1, -1, -2, -2, -1, 1, 2 }; //KNIGHT의 이동 데이터
	const int knightMovDataX[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };

	const int bishopMovDataY[4] = { 1, 1, -1, -1 };		   //BISHOP의 이동 데이터
	const int bishopMovDataX[4] = { -1, 1, -1, 1 };

	const int rookMovDataY[4] = { 1, -1, 0, 0 };			//ROOK의 이동 데이터
	const int rookMovDataX[4] = { 0, 0, 1, -1 };

	const int kingMovDataY[8] = {  1, 1, 1, 0, -1, -1, -1,  0 }; //KING의 이동 데이터
	const int kingMovDataX[8] = { -1, 0, 1, 1,  1,  0, -1, -1 }; 

	
	/* Castling을 위한 변수 */
	

	switch (curState.pieceType)
	{
	case PAWN: //현재 선택된 기물이 PAWN일 경우

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
				POS nextPos = { pos.x, pos.y + (curColor * (i + 1)) };

				if (!IsEmpty(ch, nextPos.y, nextPos.x)) break;
				if (!IsAvailableToMov(ch, nextPos.y, nextPos.x, curColor)) break;

				ret = (MOVEDATA*)realloc(ret, sizeof(MOVEDATA) * (++cnt));

				ret[cnt - 1].isCastling = FALSE;
				ret[cnt - 1].pos.y = nextPos.y;
				ret[cnt - 1].pos.x = nextPos.x;
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

	case KNIGHT: //현재 선택된 기물이 KNIGHT일 경우

		for (int i = 0; i < 8; i++)
		{
			POS nextPos = { pos.x + knightMovDataX[i], pos.y + knightMovDataY[i] };
			if (IsAvailableToMov(ch, nextPos.y, nextPos.x, curColor))
			{
				ret = (MOVEDATA*)realloc(ret, sizeof(MOVEDATA) * (++cnt));
				ret[cnt - 1].pos.y = nextPos.y;
				ret[cnt - 1].pos.x = nextPos.x;
			}
		}
		break;

	case QUEEN: //현재 선택된 기물이 QUEEN일 경우 (ROOK와 BISHOP의 case를 진행)
	case BISHOP: //현재 선택된 기물이 BISHOP일 경우

		for (int i = 0; i < 4; i++)
		{
			int plus = 1;
			POS nextPos = { pos.x + plus * bishopMovDataX[i], pos.y + plus * bishopMovDataY[i] };

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
		if (curState.pieceType != QUEEN) //QUEEN 이면, ROOK의 case도 진행.
			break;
				
	           //현재 선택된 기물이 QUEEN인 경우
	case ROOK: //현재 선택된 기물이 ROOK인 경우

		for (int i = 0; i < 4; i++)
		{
			int plus = 1;
			POS nextPos = { pos.x + plus * bishopMovDataX[i], pos.y + plus * rookMovDataY[i] };

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

	case KING: //현재 선택된 기물이 KING인 경우
		
		//일반적인 이동
		for (int i = 0; i < 8; i++)
		{
			POS nextPos = { pos.x + kingMovDataX[i], pos.y + kingMovDataY[i] };

			if (IsAvailableToMov(ch, nextPos.y, nextPos.x, curColor) && CalculateState(ch, (POS){nextPos.y, nextPos.x}))
			{
				ret = (MOVEDATA*)realloc(ret, sizeof(MOVEDATA) * (++cnt));
				ret[cnt - 1].pos.y = nextPos.y;
				ret[cnt - 1].pos.x = nextPos.x;
			}
		}

		//캐슬링
		//킹의 움직임 여부를 우선 판단
		if (ch->states[curColor == WHITE_PLAYER ? 7 : 0][curColor == WHITE_PLAYER ? 4 : 3].pieceType == KING &&
			ch->states[curColor == WHITE_PLAYER ? 7 : 0][curColor == WHITE_PLAYER ? 4 : 3].moveCnt == 0)
		{
			POS kingPos = { curColor == WHITE_PLAYER ? 7 : 0 , curColor == WHITE_PLAYER ? 4 : 3 };
			POS rookPosK = { curColor == WHITE_PLAYER ? 7 : 0, curColor == WHITE_PLAYER ? 7 : 0 };
			POS rookPosQ = { curColor == WHITE_PLAYER ? 7 : 0, curColor == WHITE_PLAYER ? 0 : 7 };

			//현재 킹이 체크되어있는지 판단
			if (CalculateState(ch, (POS) { kingPos.x, kingPos.y }) == 1)
				break;

			//킹사이드 캐슬링
			//룩의 움직임 여부 판단
			if (ch->states[rookPosK.y][rookPosK.x].pieceType == ROOK &&
				ch->states[rookPosK.y][rookPosK.x].moveCnt == 0)
			{
				int flag = FALSE;

				for (int i = 1; i <= 2; i++) //킹과 룩 사이의 빈칸에 적의 기물의 이동이 가능한지 여부 판단
				{
					POS nextPos = { kingPos.x - (curColor * i), kingPos.y };

					if (!IsEmpty(ch, nextPos.y, nextPos.x) ||
						CalculateState(ch, (POS){nextPos.x, nextPos.y}) != 0)
						flag = TRUE;
				}
				if (!flag) 
				{
					ret = (MOVEDATA*)realloc(ret, sizeof(MOVEDATA) * (++cnt));
					ret[cnt - 1].pos.y = kingPos.y;
					ret[cnt - 1].pos.x = kingPos.x - (2 * curColor);
				}
			}
			//퀸사이드 캐슬링
			//룩의 움직임 여부 판단
			if (ch->states[rookPosQ.y][rookPosQ.x].pieceType == ROOK &&
				ch->states[rookPosQ.y][rookPosQ.x].moveCnt == 0)
			{
				int flag = FALSE;

				for (int i = 1; i <= 2; i++) //킹과 룩 사이의 빈칸에 적의 기물의 이동이 가능한지 여부 판단
				{
					POS nextPos = { kingPos.x + (curColor * i), kingPos.y };

					if (!IsEmpty(ch, nextPos.y, nextPos.x) || 
						CalculateState(ch, (POS) { nextPos.x, nextPos.y }) != 0 )
						flag = TRUE;
				}
				if (!flag)
				{
					ret = (MOVEDATA*)realloc(ret, sizeof(MOVEDATA) * (++cnt));
					ret[cnt - 1].pos.y = kingPos.y;
					ret[cnt - 1].pos.x = kingPos.x + (2 * curColor);
				}
			}
		}

		break;
	}

	return ret;
}

// 기물을 움직이고 승패판정
void Move(CHESS* chess, const POS src, const MOVEDATA desMoveData) 
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


	int gameState = CalculateState(chess, kingPos); // 현재 어떤 상태인지, (Check, CheckMate, StaleMate)
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
// Check인지 CheckMate인지 StaleMate인지 판별함
int CalculateState(CHESS* chess, const POS kingPos) 
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



