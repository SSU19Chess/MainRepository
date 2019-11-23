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

typedef struct _POS //��ġ ����
{
	int x, y;
}POS;

typedef struct _MOVEDATA
{
	POS pos;
	int isCastling; //1 = ŷ���̵�, 2 = �����̵�
	int isEP; //���Ļ����� ���� �� �ִ��� ���θ� ��Ÿ����
}MOVEDATA;

typedef struct _STATEDATA
{
	PIECETYPE pieceType;
	int player;
	int moveCnt;
	int epState; //pawn ���� ���, ���Ļ����� ������ �ִ��� ���θ� ��Ÿ����.
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
	int currentPlayer; // ���� �⹰�� ������ �� �ִ� �÷��̾��� ��, (Game.c - Move �Լ�)���� �ٲ�
	GAMEPRINTINFO printInfo;
}CHESS;

