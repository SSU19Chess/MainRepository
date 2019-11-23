#pragma once
#include <Windows.h>

// Inputs
#define LEFT  75
#define RIGHT  77
#define UP   72
#define DOWN 80
#define ENTER  13

//
#define EMPTY_PLAYER 0
#define BLACK_PLAYER 1
#define WHITE_PLAYER -1

//
#define CHESS_SIZE 8

//


typedef enum _PIECETYPE { NONE, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING } PIECETYPE;

typedef struct _POS //위치 정보
{
	int x, y;
}POS;

typedef struct _MOVEDATA
{
	POS pos;
	int isCastling; //1 = 킹사이드, 2 = 퀸사이드
	int isEP; //앙파상으로 잡힐 수 있는지 여부를 나타낸다
}MOVEDATA;

typedef struct _STATEDATA
{
	PIECETYPE pieceType;
	int player;
	int moveCnt;
	int epState; //pawn 전용 멤버, 앙파상으로 잡힐수 있는지 여부를 나타낸다.
}STATEDATA;

typedef struct _GAMEPRINTINFO
{
	int gameState; // 0 == None, 1 == Check, 2 == CheckMate, 3 ==StaleMate
	PIECETYPE diedPiece[2][20];
	int diedPieceCnt[2];
}GAMEPRINTINFO;

typedef struct _CHESS
{
	STATEDATA states[CHESS_SIZE][CHESS_SIZE];
	int currentPlayer; // 현재 기물을 움직일 수 있는 플레이어의 색, (Game.c - Move 함수)에서 바뀜
	GAMEPRINTINFO printInfo;
}CHESS;

