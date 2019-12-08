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

// 초기 게임 판 구성
const PIECETYPE stalePieces[CHESS_SIZE][CHESS_SIZE] = {
	{NONE, NONE, NONE, NONE, NONE, KING, NONE, NONE},
	{NONE, NONE, NONE, NONE, NONE, PAWN, NONE, NONE},
	{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
	{NONE, NONE, NONE, NONE, NONE, KING, NONE, NONE},
	{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
	{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
	{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
	{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE}
};

/**
	@ 함수 이름: InGame
	@ 함수 설명: 게임화면, 게임 상태(Check, CheckMate, StaleMate)를 화면에 출력하고
				 사용자의 입력(방향키 상하좌우, Enter)을 받아 기물을 움직이는 함수를 호출
				 입력을 받고 게임에 상태에 따라 게임을 종료한다.
	@ 참조 함수들: Init, PrintBoard, PrintState, Input, PrintResult
	//
**/
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
		if (chess->printInfo.gameState == 2)
		{
			// 체크메이트 발생 게임 종료
			PrintBoard(chess, moveData);
			PrintState(chess);
			PrintResult(chess->currentPlayer * -1);
			return 1;
		}
		else if (chess->printInfo.gameState == 3)
		{
			// 스테일메이트 발생 게임 종료
			PrintBoard(chess, moveData);
			PrintState(chess);
			PrintResult(0);
			return 1;
		}
	}
}

/**
	@ 함수 이름: MainMenu
	@ 함수 설명: 게임 시작화면을 화면에 출력해주고 키보드 위, 아래 방향키를 통해 '게임 시작', '게임 종료'를 선택한다.
				 '게임 시작'을 선택하면 0을, '게임 종료'를 선택하면 -1을 반환한다.
	@ 참조 함수들: PrtMainMenu, gotoxy
**/
int MainMenu()
{
	// 초기 세팅 -------------------------------
	CursorView(0);

	// 변수 선언 -------------------------------
	int ypos = 20;


	// 시작화면---------------------------------
	PrtMainMenu();
	while (1)
	{
		gotoxy(ypos, 35);

		int input = _getch();
		if (input == 224)
		{
			input = _getch();
			switch (input)
			{
			case UP:
				ypos = 20;
				printf("  ");
				break;
			case DOWN:
				ypos = 22;
				printf("  ");
				break;
			}
			gotoxy(ypos, 35);
			SetColor(Red, 0);
			printf("☞");
		}
		else if (input == ENTER)
		{
			system("cls");
			CursorView(1);
			return ypos == 14 ? -1 : 0;
		}
	}
}

/**
	@ 함수 이름: Input
	@ 함수 설명: getch 함수를 이용하여 '방향키 상하좌우'를 받아 체스판에서 커서를 이동하고
	             'Enter'를 입력받아서 기물을 선택하거나, 기물을 움직인다.
	@ 파라미터 이름 나열: (chess, lastSelectedPos, currentPos, moveData)
	@ 파라미터 설명
		@ chess: 체스 게임에 대한 정보를 가지고 있는 구조체
		@ lastSelectedPos: 선택한 기물의 위치
		@ currentPos: 현재 커서의 위치
		@ moveData: 선택한 기물이 이동할 수 있는 정보를 가진 구조체 포인터를 가리키는 포인터
	@ 참조 함수들: gotoxy, IsInMoveData, Move, GetMoveData
	@ exception 예외처리
	//
**/
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
			int index = IsInMoveData(*moveData, *currentPos); // moveData가 NULL이거나, moveData 배열 안에 currentPos와 같은 pos를 가진 값이 없다면 -1, 있으면 인덱스 반환
			if (index != -1) // moveData 안에 currentPos가 있다면
			{
				Move(chess, *lastSelectedPos, *(*moveData + index)); // 기물을 움직이고
				*lastSelectedPos = (POS){ -1,-1 };	// 선택한 기물의 위치를 저장하는 변수를 초기화 한다.

				if (*moveData != NULL)
				{
					free(*moveData); // 메모리 해제
					*moveData = NULL;
				}
			}			
			else
			{
				if (chess->states[currentPos->y][currentPos->x].pieceType != NONE)					// 선택한 위치가 비어있지 않고
				{
					if (chess->states[currentPos->y][currentPos->x].player == chess->currentPlayer) // 선택한 위치의 기물의 색이 현재 턴의 플레이어의 색과 같다면
					{
						memcpy(lastSelectedPos, currentPos, sizeof(POS));	// 선택한 위치를 저장하고
						*moveData = GetMoveData(chess, *currentPos);		// 선택한 위치에 있는 기물의 이동가능한 경로를 얻는다.

						if (_msize(*moveData) / sizeof(MOVEDATA) == 0)	// 만약 기물의 이동가능한 경로가 없다면
						{
							free(*moveData);	// 할당받은 메모리를 해제해주고
							*moveData = NULL;	// 아무것도 가리키게 하지 않는다.
						}
					}
				}
				else
				{
					if (*moveData != NULL)
					{
						free(*moveData); // 메모리 해제
						*moveData = NULL;
					}
				}
			}
			r = FALSE;      //r의 값에 0을 대입 (무한루프 탈출)
			break;
		}
		}
	}
}


/**
	@ 함수 이름: Init
	@ 함수 설명: CHESS 구조체를 동적할당 받아서 체스판을 초기화하고 구조체의 정보를 초기화하고 해당 구조체를 반환한다.
	@ 참조 함수들
	@ exception 예외처리
	// CHESS 구조체 동적할당에 실패하면 NULL을 반환한다.
**/
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
	chess->currentPlayer = WHITE_PLAYER;

	chess->printInfo.gameState = 0;
	for (int i = 0; i < 2; i++)
	{
		chess->printInfo.diedPieceCnt[i] = 0;
		for (int j = 0; j < 20; j++)
		{
			chess->printInfo.diedPiece[i][j] = NONE;
		}
	}
	return chess;
}

/**
	@ 함수 이름: IsInMoveData
	@ 함수 설명: pos가 md목록 안에 있는지를 확인하여 있다면 인덱스를 반환, 없으면 -1을 반환한다.
	@ 파라미터 이름 나열 (md, pos)
	@ 파라미터 설명
		@ pos: x, y 값을 담는 구조체
		@ md: MOVEDATA 구조체 배열을 가리키는 포인터
	@ exception 예외처리
	// md가 NULL을 가리키면 -1을 반환
**/
int IsInMoveData(const MOVEDATA* md, const POS pos)
{
	if (md == NULL)
		return -1;

	size_t size = _msize(md) / sizeof(MOVEDATA);
	for (int i = 0; i < size; i++)
	{
		if (md[i].pos.x == pos.x && md[i].pos.y == pos.y)
			return i;
	}
	return -1;
}

/**
	@ 함수 이름 : IsAvailableToMov
	@ 함수 설명 : 파라미터로 전달된 말이 y, x로 이동할 수 있는지 여부 반환
	@ 파라미터 이름 나열 : (ch, y, x, curColor)
	@ 파라미터 설명 :
		@ ch : 체스 게임에 대한 정보를 가지고 있는 구조체
		@ y, x : 전달된 말의 좌표
		@ curColor : 전달된 말의 소유자
	@ 참조 함수들 : IsInMap
	@ exception 예외처리
	// 
**/
BOOL IsAvailableToMov(CHESS* ch, int y, int x, int curColor) 
{
	if (ch->states[y][x].player == curColor) //해당 칸에 같은편 기물이 있으면 FALSE
		return FALSE;

	if(!IsInMap((POS){x, y}))
		return FALSE;
	
	return TRUE;
}

/**
	@ 함수 이름 : IsEmpty
	@ 함수 설명 : 해당 칸이 비어있으면 TRUE
	@ 파라미터 이름 나열 : ch, y, x
	@ 파라미터 설명 : 
		@ ch : 체스 게임에 대한 정보를 가지고 있는 구조체
		@ y, x : 전달된 말의 좌표
	@ 참조 함수들 : IsInMap
	@ exception 예외처리 
	//
**/
BOOL IsEmpty(CHESS* ch, int y, int x)
{
	if (!IsInMap((POS) { x, y }))
		return FALSE;

	if (ch->states[y][x].pieceType == NONE)
		return TRUE;
	else
		return FALSE;
}

/**
	@ 함수 이름: IsInMap
	@ 함수 설명: pos가 체스판 범위 안에 있는지 확인하여 범위 안이면 True를 반환, 범위 밖이면 False를 반환한다.
	@ 파라미터 이름 나열 (pos)
	@ 파라미터 설명
		@ pos: x, y 값을 담는 구조체
**/
BOOL IsInMap(POS pos)
{
	if (pos.x < 0 || pos.x >= CHESS_SIZE || pos.y < 0 || pos.y >= CHESS_SIZE)
		return FALSE;
	return TRUE;
}

/**
	@ 함수 이름: OtherCanCome
	@ 함수 설명: playerColor와 다른 색의 말이 pos로 올 수 있는지를 확인하여 결과를 반환한다. 
	@ 파라미터 이름 나열 (chess, pos, playerColor)
	@ 파라미터 설명
		@ chess: 체스 게임에 대한 정보를 가지고 있는 구조체
		@ pos: playerColor와 다른 색의 말이 올 수 있는지를 확인하기 위한 위치
		@ playerColor: 검사할 플레이어의 색깔
		@ isEmpty: 해당 칸이 앞으로 비어있는지를 표시하는 변수( 폰이 올 수 있는지를 확인하기 위헤)
	@ 참조 함수들: IsInMap
	@ exception 예외처리
	// 탐색 중 체스판의 인덱스를 넘어갈 경우 for문 내에서 break를 건다.
**/
BOOL OtherCanCome(CHESS* chess, const POS pos, int playerColor, BOOL isEmpty)
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
						if (i == 1 && !isEmpty)
						{
							if (playerColor == BLACK_PLAYER)
							{
								// 검은색일 때 상대의 말이 흰색이므로 
								// 상대가 위쪽 대각선 방향으로 공격하므로
								// 검색당하는 검은색은 아래쪽 대각선을 검사
								if ((d == 2 && s == 1) || (d == 3 && s == -1))
									return TRUE;
							}
							else
							{
								if ((d == 2 && s == -1) || (d == 3 && s == 1))
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

/**
	@ 함수 이름: CheckAround
	@ 함수 설명: King이 주위 8칸으로 움직일 수 없다면 True, 있으면 False를 반환한다.
	@ 파라미터 이름 나열 (chess, pos, playerColor)
	@ 파라미터 설명
		@ chess: 체스 게임에 대한 정보를 가지고 있는 구조체
		@ pos: playerColor와 다른 색의 말이 올 수 있는지를 확인하기 위한 위치
		@ playerColor: 검사할 플레이어의 색깔
	@ 참조 함수들: IsInMap
	@ exception 예외처리
	// 탐색 중 체스판의 인덱스를 넘어갈 경우 for문 내에서 break를 건다.
**/
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

			// 주위에 상대편 킹이 있는지 확인
			BOOL canMoveHere = TRUE;
			for (int j = -1; j <= 1; j++)
			{
				for (int i = -1; i <= 1; i++)
				{
					if (i == 0 && j == 0)
						break;
					POS __kingPos = { kingPos.x + x + i, kingPos.y + y + j};
					// 맵 밖도 확인할 필요 없음
					if (!IsInMap(__kingPos))
						continue;

					if (chess->states[__kingPos.y][__kingPos.x].pieceType == KING &&
						chess->states[__kingPos.y][__kingPos.x].player != chess->states[kingPos.y][kingPos.x].player)
					{
						canMoveHere = FALSE;
						break;
					}
				}
				if (!canMoveHere)
					break;
			}
			if (!canMoveHere)
				continue;


			if (chess->states[_kingPos.y][_kingPos.x].pieceType != NONE)
			{
				// 다른 색의 기물이 있다면 aroundOnlySameColor를 False로 
				if (chess->states[_kingPos.y][_kingPos.x].player != chess->states[kingPos.y][kingPos.x].player)
				{
					if (!OtherCanCome(chess, _kingPos, chess->states[kingPos.y][kingPos.x].player, FALSE))
						return FALSE;
					aroundOnlySameColor = FALSE;
				}
				continue;
			}

			aroundIsFull = FALSE; // 위 if문을 토대로 주변에 빈 공간이 있음을 알 수 있음

			if (!OtherCanCome(chess, _kingPos, chess->states[kingPos.y][kingPos.x].player, FALSE))
				return FALSE;
		}
	}
	// 주변에 빈 공간이 없더라도 다 같은 색이면, 움직일 수 없어도 상관 없음
	if (aroundOnlySameColor && aroundIsFull)
		return FALSE;
	return TRUE;

}

/**
	@ 함수 이름: PiecesCanMove
	@ 함수 설명: curColor와 같은 색의 플레이어의 기물들이 움직일 수 있는 수가 있는지 확인하여 가능하면 True, 움직일 수 없다면 False를 반환한다.
	@ 파라미터 이름 나열 (chess, curColor)
	@ 파라미터 설명
		@ chess: 체스 게임에 대한 정보를 가지고 있는 구조체
		@ curColor: 확인할 플레이어의 색깔
	@ 참조 함수들: GetMoveData
**/
BOOL PiecesCanMove(CHESS* chess, int curColor)
{
	for (int y = 0; y < CHESS_SIZE; y++)
	{
		for (int x = 0; x < CHESS_SIZE; x++)
		{
			if (chess->states[y][x].player == curColor)
			{
				MOVEDATA* m = GetMoveData(chess, (POS) { x, y });
				size_t t = _msize(m) / sizeof(MOVEDATA);
				if (t != 0)
					return TRUE;
			}
		}
	}
	return FALSE;
}

/**
	@ 함수 이름: GetKingPos
	@ 함수 설명: curColor의 King 위치를 찾아서 반환한다.
	@ 파라미터 이름 나열 (chess, curColor)
	@ 파라미터 설명
		@ chess : 체스 게임에 대한 정보를 가지고 있는 구조체
		@ src : 전달된 플레이어의 색깔
**/
POS GetKingPos(CHESS* chess, int curColor)
{
	POS ret;

	//완전 탐색으로 KING의 위치를 찾는다.
	for (int y = 0; y < CHESS_SIZE; y++)
	{
		for (int x = 0; x < CHESS_SIZE; x++)
		{
			if (chess->states[y][x].player == curColor &&
				chess->states[y][x].pieceType == KING)
			{
				ret = (POS){ x, y };
				return ret;
			}
		}
	}

	return (POS) { -1, -1 }; //ERROR!
}

/**
	@ 함수 이름: GetMoveData
	@ 함수 설명: 매개변수로 전달된 체스판을 기반으로 이동가능한 위치 데이터들을 반환
	@ 파라미터 이름 나열 (ch, pos)
	@ 파라미터 설명
		@ ch : 체스 게임에 대한 정보를 가지고 있는 구조체
		@ pos : x, y 값을 담는 구조체
	@ 참조 함수들 : GetKingPos, CalculateState, Move, IsAvailableToMov, IsEmpty, OtherCanCome
	@ exception 예외처리
	//
**/
MOVEDATA* GetMoveData(CHESS* ch, const POS pos)
{
	MOVEDATA* mvData; 
	MOVEDATA* chkedData; //체크 되어있을때, 반환값

	const STATEDATA curState = ch->states[pos.y][pos.x];
	const int curColor = curState.player;			 //현재 플레이어의 color
	const int oppoColor = curColor * -1;			 //상대 플레이어의 color

	int cnt = 0;									 //mvData의 길이 즉, 이동가능한 위치의 개수
	mvData = (MOVEDATA*)malloc(sizeof(MOVEDATA) * cnt); //길이가 0인 빈 공간 할당

	POS curKingPos = GetKingPos(ch, curColor);

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

		for (int i = 1; ; i *= -1) //앙파상
		{
			if (ch->states[pos.y][pos.x + i].epState == TRUE)
			{
				mvData = (MOVEDATA*)realloc(mvData, sizeof(MOVEDATA) * (++cnt));
				mvData[cnt - 1].pos.y = pos.y + curColor;
				mvData[cnt - 1].pos.x = pos.x + i;
				mvData[cnt - 1].isEP = TRUE; //앙파상으로 잡힐 수 있는 상태
			}
			
			if (i == -1) break;
		}

		for (int i = 1; ; i *= -1) //대각선에 적 기물이 있는지 판단
		{
			if (ch->states[pos.y + curColor][pos.x + i].player == oppoColor) 
			{
				if (IsAvailableToMov(ch, pos.y + curColor, pos.x + i, curColor))
				{
					mvData = (MOVEDATA*)realloc(mvData, sizeof(MOVEDATA) * (++cnt));
					mvData[cnt - 1].pos.y = pos.y + curColor;
					mvData[cnt - 1].pos.x = pos.x + i;
				}
			}
			if (i == -1) break;
		}
		if (curState.moveCnt == 0) //Pawn이 한번도 움직이지 않았을 경우
		{
			for (int i = 0; i <= 1; i++)
			{
				POS nextPos = { pos.x, pos.y + (curColor * (i + 1)) };

				if (!IsEmpty(ch, nextPos.y, nextPos.x)) break;
				if (!IsAvailableToMov(ch, nextPos.y, nextPos.x, curColor)) break;

				mvData = (MOVEDATA*)realloc(mvData, sizeof(MOVEDATA) * (++cnt));

				mvData[cnt - 1].isCastling = FALSE;
				mvData[cnt - 1].pos.y = nextPos.y;
				mvData[cnt - 1].pos.x = nextPos.x;
			}
		}
		else //Pawn이 한번 이상 움직였을 경우
		{
			if (!IsEmpty(ch, pos.y + curColor, pos.x)) break;

			mvData = (MOVEDATA*)realloc(mvData, sizeof(MOVEDATA) * (++cnt));
			mvData[cnt - 1].pos.y = pos.y + curColor;
			mvData[cnt - 1].pos.x = pos.x;
		}
		break;

	case KNIGHT: //현재 선택된 기물이 KNIGHT일 경우

		for (int i = 0; i < 8; i++)
		{
			POS nextPos = { pos.x + knightMovDataX[i], pos.y + knightMovDataY[i] };
			if (IsAvailableToMov(ch, nextPos.y, nextPos.x, curColor))
			{
				mvData = (MOVEDATA*)realloc(mvData, sizeof(MOVEDATA) * (++cnt));
				mvData[cnt - 1].pos.y = nextPos.y;
				mvData[cnt - 1].pos.x = nextPos.x;
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
				mvData = (MOVEDATA*)realloc(mvData, sizeof(MOVEDATA) * (++cnt));
				mvData[cnt - 1].pos.y = nextPos.y;
				mvData[cnt - 1].pos.x = nextPos.x;

				if (ch->states[nextPos.y][nextPos.x].player == oppoColor) break;

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
			POS nextPos = { pos.x + plus * rookMovDataX[i], pos.y + plus * rookMovDataY[i] };

			while (IsAvailableToMov(ch, nextPos.y, nextPos.x, curColor))
			{
				mvData = (MOVEDATA*)realloc(mvData, sizeof(MOVEDATA) * (++cnt));
				mvData[cnt - 1].pos.x = nextPos.x;
				mvData[cnt - 1].pos.y = nextPos.y;

				if (ch->states[nextPos.y][nextPos.x].player == oppoColor) break;

				nextPos.x = pos.x + plus * rookMovDataX[i];
				nextPos.y = pos.y + plus * rookMovDataY[i];
				plus++;
			}
		}
		break;

	case KING: //현재 선택된 기물이 KING인 경우
		
		//일반적인 이동
		for (int i = 0; i < 8; i++)
		{
			POS nextPos = { pos.x + kingMovDataX[i], pos.y + kingMovDataY[i] };

			// 주위에 상대편 킹이 있는지 확인
			BOOL canMoveHere = TRUE;
			for (int j = -1; j <= 1; j++)
			{
				for (int i = -1; i <= 1; i++)
				{
					if (i == 0 && j == 0)
						break;
					POS __kingPos = { nextPos.x + i, nextPos.y + j };
					// 맵 밖도 확인할 필요 없음
					if (!IsInMap(__kingPos))
						continue;

					if (ch->states[__kingPos.y][__kingPos.x].pieceType == KING &&
						ch->states[__kingPos.y][__kingPos.x].player != curColor)
					{
						canMoveHere = FALSE;
						break;
					}
				}
				if (!canMoveHere)
					break;
			}


			if (IsAvailableToMov(ch, nextPos.y, nextPos.x, curColor) && !OtherCanCome(ch, (POS){nextPos.x, nextPos.y}, curColor, FALSE) && canMoveHere)
			{
				mvData = (MOVEDATA*)realloc(mvData, sizeof(MOVEDATA) * (++cnt));
				mvData[cnt - 1].pos.y = nextPos.y;
				mvData[cnt - 1].pos.x = nextPos.x;
			}
		}
		
		//캐슬링
		//킹의 움직임 여부를 우선 판단
		if (ch->states[curColor == WHITE_PLAYER ? 7 : 0][curColor == WHITE_PLAYER ? 4 : 4].pieceType == KING &&
			ch->states[curColor == WHITE_PLAYER ? 7 : 0][curColor == WHITE_PLAYER ? 4 : 4].moveCnt == 0)
		{
			POS kingPos =  curColor == WHITE_PLAYER ? (POS) {4, 7} : (POS) {4, 0} ;
			POS rookPosK =  curColor == WHITE_PLAYER ? (POS) {7, 7} : (POS) {7, 0} ;
			POS rookPosQ =  curColor == WHITE_PLAYER ? (POS) {0, 7} : (POS) {0, 0} ;

			//현재 킹이 체크되어있는지 판단
			if (CalculateState(ch, (POS) { kingPos.x, kingPos.y }) == 1)
				break;

			//킹사이드 캐슬링
			//룩의 움직임 여부 판단
			if (ch->states[rookPosK.y][rookPosK.x].pieceType == ROOK &&
				ch->states[rookPosK.y][rookPosK.x].moveCnt == 0)
			{
				int flag = FALSE; //flag가 true? : 캐슬링 조건 만족 X

				for (int i = 1; i <= 2; i++) //킹과 룩 사이의 빈칸에 적의 기물의 이동이 가능한지 여부 판단
				{
					POS nextPos = { kingPos.x + i, kingPos.y };

					if (!IsEmpty(ch, nextPos.y, nextPos.x) ||
						OtherCanCome(ch, (POS) {nextPos.x, nextPos.y}, curColor,FALSE))
						flag = TRUE;
				}
				if (!flag) 
				{
					mvData = (MOVEDATA*)realloc(mvData, sizeof(MOVEDATA) * (++cnt));
					mvData[cnt - 1].pos.y = kingPos.y;
					mvData[cnt - 1].pos.x = kingPos.x + 2;
					mvData[cnt - 1].isCastling = 1;
				}
			}
			
			//퀸사이드 캐슬링
			//룩의 움직임 여부 판단
			if (ch->states[rookPosQ.y][rookPosQ.x].pieceType == ROOK &&
				ch->states[rookPosQ.y][rookPosQ.x].moveCnt == 0)
			{
				int flag = FALSE; //flag가 true? : 캐슬링 조건 만족 X

				for (int i = 1; i <= 2; i++) //킹과 룩 사이의 빈칸에 적의 기물의 이동이 가능한지 여부 판단
				{
					POS nextPos = { kingPos.x - i, kingPos.y };

					if (!IsEmpty(ch, nextPos.y, nextPos.x) || 
						OtherCanCome(ch, (POS) { nextPos.x, nextPos.y }, curColor, FALSE))
						flag = TRUE;
				}
				if (!flag)
				{
					mvData = (MOVEDATA*)realloc(mvData, sizeof(MOVEDATA) * (++cnt));
					mvData[cnt - 1].pos.y = kingPos.y;
					mvData[cnt - 1].pos.x = kingPos.x - 2;
					mvData[cnt - 1].isCastling = 2;
				}
			}
		}
		

		break;
	}

	//킹이 체크인경우 mvData 검사
	//mvData에서 체크를 벗어나게 하는 수만을 chkedData에 넣어 반환
	if (ch->printInfo.gameState == 1)
	{
		CHESS ch2;
		ch2 = *ch;

		chkedData = (MOVEDATA*)malloc(sizeof(MOVEDATA) * cnt);
		cnt = 0;

		for (size_t idx = 0; idx < _msize(mvData) / sizeof(MOVEDATA); idx++)
		{
			Move(&ch2, pos, *(mvData+idx));
			curKingPos = GetKingPos(&ch2, curColor);
			if (CalculateState(&ch2, curKingPos) == 0)
			{
				chkedData = (MOVEDATA*)realloc(chkedData, sizeof(MOVEDATA) * (++cnt));
				chkedData[cnt - 1].pos = mvData[idx].pos;
				chkedData[cnt - 1].isCastling = mvData[idx].isCastling;
				chkedData[cnt - 1].isEP = mvData[idx].isEP;
			}
			ch2 = *ch;
		}

		return chkedData;
	}
	else
		return mvData;
}

// King이 공격을 받지 않음 == 0
// Check == 1
// 상대의 공격을 막을 수 없음 == 2
// Check인지 CheckMate인지 StaleMate인지 판별함
/**
	@ 함수 이름: CalculateState
	@ 함수 설명: 현재 chess 구조체를 기반으로 게임의 상태를 계산하여 반환한다.
				 King이 상대편 기물에게 공격을 받을 위기가 아니면 0을 반환
				 Check상태이면 1을 반환
				 King이 상대편 기물에게 공격을 받을 위기인데 막을 수 없다면 2를 반환한다.
	@ 파라미터 이름 나열 (chess, kingPosa)
	@ 파라미터 설명
		@ chess: 체스 게임에 대한 정보를 가지고 있는 구조체
		@ kingPos: King의 위치
	@ 참조 함수들: IsInMap, OtherCanCome
**/
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
							if (OtherCanCome(chess, _pos, color,TRUE))
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
							if (OtherCanCome(chess, _pos, color, TRUE))
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
								if ((d == 2 && s == -1) || (d == 3 && s == -1))
								{
									existEnemy = TRUE;
									int color = kingColor == BLACK_PLAYER ? WHITE_PLAYER : BLACK_PLAYER;
									if (OtherCanCome(chess, _checkPos, color, TRUE))
										return 1;
								}
							}
							else
							{
								if ((d == 2 && s == 1) || (d == 3 && s == 1))
								{
									existEnemy = TRUE;
									int color = kingColor == BLACK_PLAYER ? WHITE_PLAYER : BLACK_PLAYER;
									if (OtherCanCome(chess, _checkPos, color, TRUE))
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
					if (OtherCanCome(chess, _checkPos, color, TRUE))
						return 1;
				}
			}
		}
	}
	if (existEnemy)
		return 2;
	return 0;
}

/**
	@ 함수 이름: Move
	@ 함수 설명: src에 있는 기물을 desMoveData.pos로 움직인다. 기물의 타입에 따라, King의 경우 캐슬링, Pawn의 경우 앙파상 여부를 확인하고 시행한다.
				 기물을 움직이고 게임의 상태( Check, CheckMate, StaleMate)를 확인한다.
				 위 작업이 끝난 후 상대방에게 턴을 넘긴다.
	@ 파라미터 이름 나열 (chess, src, desMoveData)
	@ 파라미터 설명
		@ chess: 체스 게임에 대한 정보를 가지고 있는 구조체
		@ src: 움직일 기물의 원래 위치
		@ desMoveData: 기물이 움직일 위치에 관한 정보를 가진 MOVEDATA 구조체
	@ 참조 함수들: UpdateEpState, Promotion, GetKingPos, CalculateState, CheckAround, PiecesCanMove
**/
void Move(CHESS* chess, const POS src, const MOVEDATA desMoveData)
{
	UpdateEpState(chess, chess->states[src.y][src.x].player); //앙파상 폰 관리

	int curPlayer = chess->states[src.y][src.x].player;

	// 움직일 위치에 상대 말이 있는 경우
	if (chess->states[desMoveData.pos.y][desMoveData.pos.x].pieceType != NONE)
	{
		int playerIndex = chess->states[src.y][src.x].player == BLACK_PLAYER ? 1 : 0;
		chess->printInfo.diedPiece[playerIndex][chess->printInfo.diedPieceCnt[playerIndex]++] = chess->states[desMoveData.pos.y][desMoveData.pos.x].pieceType;
	}

	switch (chess->states[src.y][src.x].pieceType)
	{
	case KING: // King일 경우 캐슬링 확인
		if (desMoveData.isCastling == 1 || desMoveData.isCastling == 2)
		{
			//King 이동
			chess->states[src.y][src.x].moveCnt++;
			chess->states[desMoveData.pos.y][desMoveData.pos.x] = chess->states[src.y][src.x];
			chess->states[src.y][src.x] = (STATEDATA){ .pieceType = NONE, .player = EMPTY_PLAYER, .moveCnt = 0 }; // 움직이기 전의 위치는 NONE으로

			//Rook 이동
			POS rookPos = { 0, 0 };
			POS rookDesPos = { 0, 0 };

			if (desMoveData.isCastling == 1) //킹 사이드 캐슬링인 경우
			{
				rookPos = (curPlayer == WHITE_PLAYER ? (POS) { 7, 7 } : (POS) { 7, 0 });
				rookDesPos = (curPlayer == WHITE_PLAYER ? (POS) { 5, 7 } : (POS) { 5, 0 });
			}
			else if (desMoveData.isCastling == 2) // 퀸 사이드 캐슬링인 경우
			{
				rookPos = (curPlayer == WHITE_PLAYER ? (POS) { 0, 7 } : (POS) { 0, 0 });
				rookDesPos = (curPlayer == WHITE_PLAYER ? (POS) { 3, 7 } : (POS) { 3, 0 });
			}

			chess->states[rookPos.y][rookPos.x].moveCnt++;
			chess->states[rookDesPos.y][rookDesPos.x] = chess->states[rookPos.y][rookPos.x];
			chess->states[rookPos.y][rookPos.x] = (STATEDATA){ .pieceType = NONE, .player = EMPTY_PLAYER, .moveCnt = 0 }; // 움직이기 전의 위치는 NONE으로			

			break;
		}


	case PAWN:
		chess->states[src.y][src.x].moveCnt++;
		chess->states[desMoveData.pos.y][desMoveData.pos.x] = chess->states[src.y][src.x]; // 기물을 움직이고

		//앙파상을 당할수 있는지 판단하는 변수를 업데이트
		if (chess->states[src.y][src.x].moveCnt == 1 &&								 //최초로 움직이는 경우이고,
			desMoveData.pos.y == (chess->states[src.y][src.x].player == WHITE_PLAYER ? 4 : 3)) //2칸 앞으로 전진하는 경우
			chess->states[desMoveData.pos.y][desMoveData.pos.x].epState = TRUE;

		chess->states[src.y][src.x] = (STATEDATA){ .pieceType = NONE, .player = EMPTY_PLAYER, .moveCnt = 0 }; // 움직이기 전의 위치는 NONE으로

		//앙파상 조건을 만족하여 상대방 폰의 뒤로 이동할 경우, 
		if (desMoveData.isEP == TRUE) 
		{
			int playerIndex = chess->states[src.y][src.x].player == BLACK_PLAYER ? 1 : 0;
			chess->printInfo.diedPiece[playerIndex][chess->printInfo.diedPieceCnt[playerIndex]++] = chess->states[desMoveData.pos.y - curPlayer][desMoveData.pos.x].pieceType;
			//상대방의 기물을 제거.
			chess->states[desMoveData.pos.y - curPlayer][desMoveData.pos.x] = (STATEDATA){ .pieceType = NONE, .player = EMPTY_PLAYER, .moveCnt = 0 }; //앙파상 당한 폰의 위치는 NONE으로			
		}

		//프로모션 조건을 만족할 경우
		if (desMoveData.pos.y == (curPlayer == WHITE_PLAYER ? 0 : 7))
			Promotion(chess, desMoveData.pos, curPlayer);

		break;

	default: {
		chess->states[src.y][src.x].moveCnt++;
		chess->states[desMoveData.pos.y][desMoveData.pos.x] = chess->states[src.y][src.x]; // 기물을 움직이고
		chess->states[src.y][src.x] = (STATEDATA){ .pieceType = NONE, .player = EMPTY_PLAYER, .moveCnt = 0 }; // 움직이기 전의 위치는 NONE으로
	}
	}

	// King의 위치를 찾는 부분------------
	// 움직인 플레이어의 색이 아닌 상대의 색
	POS kingPos = GetKingPos(chess, chess->currentPlayer * -1);
	//-----------------------------------


	int gameState = CalculateState(chess, kingPos); // 현재 어떤 상태인지, (Check, CheckMate, StaleMate)
	gotoxy(0, 31);
	if (gameState == 1)
	{
		//Check
		chess->printInfo.gameState = 1;
	}
	else if (gameState == 2)
	{
		if (CheckAround(chess, kingPos))
		{
			// CheckMate
			chess->printInfo.gameState = 2;
		}
		else
		{
			//Check
			chess->printInfo.gameState = 1;
		}
	}
	else
	{
		if (CheckAround(chess, kingPos) && !PiecesCanMove(chess, chess->currentPlayer * -1))
		{
			// StaleMate
			chess->printInfo.gameState = 3;
		}
		else
		{
			//NONE
			chess->printInfo.gameState = 0;
		}
	}


	chess->currentPlayer = chess->currentPlayer == BLACK_PLAYER ? WHITE_PLAYER : BLACK_PLAYER;

}

/**
	@ 함수 이름: UpdataEpState
	@ 함수 설명: 앙파상 상태를 업데이트 한다.
	            보드 위의 앙파상이 가능한 "현재 색깔 PAWN들"을 탐색하여 
				한 턴이 지나고, 앙파상이 불가능 하도록 변수를 업데이트 한다.
	@ 파라미터 이름 나열 (chess, curColor)
	@ 파라미터 설명
		@ chess : 체스 게임에 대한 정보를 가지고 있는 구조체
		@ curColor : 방금 턴을 진행한 쪽의 플레이어색
**/
void UpdateEpState(CHESS* chess, int curColor)
{
	for (int y = 0; y < CHESS_SIZE; y++)
	{
		for (int x = 0; x < CHESS_SIZE; x++)
		{
			if( chess->states[y][x].player == curColor * -1 &&
				chess->states[y][x].pieceType == PAWN &&
				chess->states[y][x].epState == TRUE)
			{
				chess->states[y][x].epState = FALSE;
			}
		}
	}
}

/**
	@ 함수 이름: Promotion
	@ 함수 설명: 해당 Pawn을 프로모션한다
	@ 파라미터 이름 나열 (chess, curPos, curColor)
	@ 파라미터 설명
		@ chess : 체스 게임에 대한 정보를 가지고 있는 구조체
		@ curPos : 프로모션 할 Pawn의 위치
 		@ curColor : 프로모션 할 Pawn의 소유자
	@ 참조 함수들 : CursorView
**/
void Promotion(CHESS* chess, POS curPos, int curColor)
{
	CursorView(0);
	PrintPromoSel(curColor);

	int ypos = GAP + 12;
	int xpos = (GAP + CHESS_SIZE * GRID_SIZE + 0) * 2 + 6;
	const int limXL = (GAP + CHESS_SIZE * GRID_SIZE + 0) * 2 + 6;
	const int limXR = (GAP + CHESS_SIZE * GRID_SIZE + 6) * 2 + 6;

	PIECETYPE piece[4] = { KNIGHT, BISHOP, ROOK, QUEEN };
	int sel = 0;
	
	while (1)
	{
		gotoxy(ypos, xpos);

		int input = _getch();
		
		// 선택
		if (input == 224)
		{
			input = _getch();
			switch (input)
			{
			case RIGHT:
				if (xpos < limXR) { xpos += 4; sel++; }
				printf("  ");
				break;
			case LEFT:
				if (xpos > limXL) { xpos -= 4; sel--; }
				printf("  ");
				break;
			}
			gotoxy(ypos, xpos);
			SetColor(White, 0);
			printf("↑");
		}

		// 선택한 기물로 PIECETYPE을 바꾼다.
		else if (input == ENTER)
		{   
			chess->states[curPos.y][curPos.x].pieceType = piece[sel];
			break;
		}
	}
	CursorView(1);

	//Erase
	for (int y = GAP + 11; y <= GAP + 12; y++)
	{
		gotoxy(y, (GAP + CHESS_SIZE * GRID_SIZE + 2) * 2);
		printf("                ");
	}
}