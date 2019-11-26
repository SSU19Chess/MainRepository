#include <stdio.h>
#include "Print.h"
#include "Game.h"
#include "GameData.h"

// 기물을 표시하는 문자
const char chessPiece[2][6] = {
	{'P', 'N', 'B', 'R', 'Q', 'K'},
	{'p', 'n', 'b', 'r', 'q', 'k'}
};

void SetColor(WORD text, WORD back) //text와 background의 색상을 변경한다.
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text + back * 16);
}

void gotoxy(int y, int x) //gotoxy 함수
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void CursorView(char show)//커서숨기기
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

/**
	@ 함수 이름: PrintBoard
	@ 함수 설명: 기본적인 체스판, 랭크와 파일을 화면에 보여주며 
				 chess 구조체를 기반으로 기물을 표시하고 md 구조체를 기반으로 기물이 움직일 수 있는 위치를 표시한다.
	@ 파라미터 이름 나열 (chess, md)
	@ 파라미터 설명
		@ chess: 체스 게임에 대한 정보를 가지고 있는 구조체
		@ md: 특정 기물의 움직일 수 있는 위치에 대한 정보를 가진 구조체 배열
	@ 참조 함수들: SetColor, gotoxy, PrintGrid
**/
void PrintBoard(CHESS* chess,const MOVEDATA* md) // 체스판 전체를 출력하는 함수
{
	int grid = 1;

	SetColor(Lgray, Black);

	// Y축 번호 UI
	for (int y = 0; y < CHESS_SIZE; y++)
	{
		gotoxy( GAP + y * GRID_SIZE + 1, GAP);
		printf("%d", 8 - y);
		// 체스판 외곽선
		for (int i = 0; i < 3; i++)
		{
			gotoxy( GAP + y * GRID_SIZE + i, GAP + 1);
			printf("┃");
			gotoxy( GAP + y * GRID_SIZE + i, GAP + 3 + CHESS_SIZE * GRID_SIZE * 2);
			printf("┃");

		}
	}

	// X축 알파벳 UI
	for (int x = 0; x < CHESS_SIZE; x++)
	{
		gotoxy( GAP / 2, (GAP + x * GRID_SIZE + 1) * 2);
		printf("%c", 'A' + x);

		// 체스판 외곽선
		for (int i = 0; i < 3; i++)
		{
			gotoxy( GAP / 2 + 1, (GAP + x * GRID_SIZE + i) * 2);
			printf("━");
			gotoxy( GAP / 2 + 2 + CHESS_SIZE * GRID_SIZE, (GAP + x * GRID_SIZE + i) * 2);
			printf("━");
		}
	}
	// 모서리 
	gotoxy( GAP - 1, (GAP - 1) * 2);
	printf("┏");
	gotoxy( GAP - 1, (GAP + CHESS_SIZE * GRID_SIZE) * 2);
	printf("┓");
	gotoxy( GAP + CHESS_SIZE * GRID_SIZE, (GAP - 1) * 2);
	printf("┗");
	gotoxy( GAP + CHESS_SIZE * GRID_SIZE, (GAP + CHESS_SIZE * GRID_SIZE) * 2);
	printf("┛");



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
	@ 함수 이름: PrintGrid
	@ 함수 설명: 기본적인 체스판에서 격자 1칸을 그리는 함수이다.
	@ 파라미터 이름 나열 (pos, sd, color, canMove)
	@ 파라미터 설명
		@ pos: 격자의 위치로 x, y 모두 1 ~ CHESS_SIZE 범위 안에 있다.
		@ sd: pos 위치에 있는 체스판의 정보로 플레이어의 색깔, 기물의 종류 등을 가지고 있다.
		@ color: 격자의 색깔을 가지고 있다. 흰색 or 검은색
		@ canMove: 현재 위치의 격자가 기물이 움직일 수 있는지를 알려준다. canMove가 -1이 아니면 격자 중앙이 다른 색으로 표시된다.
	@ 참조 함수들: SetColor, gotoxy
**/
void PrintGrid(POS pos, STATEDATA sd, COLOR color, int canMove) // 체스판에서 격자 1개를 출력하는 함수
{
	/*
	//┏┓┃┗┛━
	wchar_t list[9] = {
		L'┏', L'━', L'┓',
		L'┃', L'  ', L'┃',
		L'┗', L'━', L'┛'
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
	@ 함수 이름: PrintState
	@ 함수 설명: 현재 어떤 플레이어가 수를 두어야하는 차례인지, 각 플레이어가 잡은 기물은 무엇이 있는지를 표시한다.
	@ 파라미터 이름 나열 (chess)
	@ 파라미터 설명
		@ chess: 체스 게임에 대한 정보를 가지고 있는 구조체
	@ 참조 함수들: SetColor, gotoxy, PrintRect
**/
void PrintState(CHESS* chess) // 체스판 옆에 게임에 대한 추가적인 정보를 표시하는 함수
{
	// 현재 플레이어가 어떤 색인지 표시-------------------------------------
	PrintRect((POS){ 1, -1 }, 5, 1);
	// 글자 표시
	gotoxy( GAP, (GAP + CHESS_SIZE * GRID_SIZE + 2) * 2);
	printf("%s", chess->currentPlayer == BLACK_PLAYER ? "BLACK" : "WHITE");


	// 두 플레이어가 잡은 기물 표시-------------------------------------

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
	// Check 등등 표시

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
	@ 함수 이름: PrintResult
	@ 함수 설명: 승리 또는 무승부를 표시하는 함수이다.
	@ 파라미터 이름 나열 (winPlayerColor)
	@ 파라미터 설명
		@ winPlayerColor: 승리한 플레이어의 색깔을 표시한다, 0은 무승부이다.
	@ 참조 함수들: SetColor, gotoxy, PrintRect
**/
void PrintResult(int winPlayerColor) // BLACK_PLAYER, WHITE_PLAYER, ( 0 == 무승부)
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
	@ 함수 이름: PrintRect
	@ 함수 설명: 화면에 특수 기호를 사용하여 속이 빈 사각형을 표시하는 함수이다.
	@ 파라미터 이름 나열 (pos, width, height )
	@ 파라미터 설명
		@ pos: 사각형의 왼쪽 상단의 위치로, x 좌표는 체스판 우측 상단을 0으로 시작한다.
		@ width: 사각형 내부의 너비
		@ height: 사각형 내부의 높이
	@ 참조 함수들: SetColor, gotoxy
**/
void PrintRect(const POS pos,const int _width, const int height)
{
	int i;
	int mod = _width % 2;
	int width = _width / 2 + mod;

	SetColor(Lgray, Black);
	// 위, 아래 면
	for (i = 1; i <= width; i++)
	{
		gotoxy(GAP + pos.y, (GAP + CHESS_SIZE * GRID_SIZE + pos.x + i) * 2);
		printf("━ ");

		gotoxy(GAP + pos.y + height + 1, (GAP + CHESS_SIZE * GRID_SIZE + pos.x + i) * 2);
		printf("━ ");
	}
	// 모서리 그리기
	gotoxy(GAP + pos.y, (GAP + CHESS_SIZE * GRID_SIZE + pos.x) * 2);
	printf("┏ ");
	gotoxy(GAP + pos.y, (GAP + CHESS_SIZE * GRID_SIZE + pos.x + i) * 2 - mod);
	printf("┓ ");
	gotoxy(GAP + pos.y + height + 1, (GAP + CHESS_SIZE * GRID_SIZE + pos.x) * 2);
	printf("┗ ");
	gotoxy(GAP + pos.y + height + 1, (GAP + CHESS_SIZE * GRID_SIZE + pos.x + i) * 2 - mod);
	printf("┛ ");

	// 옆면
	for (i = 1; i <= height; i++)
	{
		gotoxy(GAP + pos.y + i, (GAP + CHESS_SIZE * GRID_SIZE + pos.x) * 2);
		printf("┃ ");
		gotoxy(GAP + pos.y + i, (GAP + CHESS_SIZE * GRID_SIZE + pos.x + width + 1) * 2 - mod);
		printf("┃ ");
	}
}

void PrtMainMenu()
{
	printf("　◆◆◆◆　◆◆◆◆　◆　　◆　　　　　　　　　\n");
	printf("　◆　　　　◆　　　　◆　　◆　　　◆◆　　　　\n");
	printf("　◆◆◆◆　◆◆◆◆　◆　　◆　　◆　　◆　　◆\n");
	printf("　　　　◆　　　　◆　◆　　◆　　　　　　◆◆　\n");
	printf("　◆◆◆◆　◆◆◆◆　　◆◆　　　　　　　　　　\n\n");
	printf("　　◆◆◆　◆　　◆　◆◆◆◆　◆◆◆◆　◆◆◆◆\n");
	printf("　◆　　　　◆　　◆　◆　　　　◆　　　　◆　　　\n");
	printf("　◆　　　　◆◆◆◆　◆◆◆◆　◆◆◆◆　◆◆◆◆\n");
	printf("　◆　　　　◆　　◆　◆　　　　　　　◆　　　　◆\n");
	printf("　　◆◆◆　◆　　◆　◆◆◆◆　◆◆◆◆　◆◆◆◆\n\n\n");
	printf("　　　　　　　게임시작　　　　　　　　　　　　　　\n");
	printf("　　　　　　　게임종료\n");


	gotoxy(13, 10);
	printf("☞");
}