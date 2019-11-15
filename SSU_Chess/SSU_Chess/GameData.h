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


typedef enum _PIECETYPE { NONE, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING } PIECETYPE;

typedef struct _POS //위치 정보
{
	int x, y;
}POS;

typedef struct _MOVEDATA
{
	POS pos;
	int isCastling;
}MOVEDATA;

typedef struct _STATEDATA
{
	PIECETYPE pieceType;
	int player;
	int moveCnt;
}STATEDATA;

typedef struct _CHESS
{
	STATEDATA states[CHESS_SIZE][CHESS_SIZE];

	int currentPlayer; // 현재 기물을 움직일 수 있는 플레이어의 색

	BOOL queenSideCastling[2];
	BOOL kingSideCastling[2];
}CHESS;