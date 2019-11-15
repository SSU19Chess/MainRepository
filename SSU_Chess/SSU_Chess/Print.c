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
void PrintBoard(CHESS* chess, MOVEDATA* md) // 체스판 전체를 출력하는 함수
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
			COLOR color = grid == 1 ? Lgray : Black;
			int canMove = IsInMoveData(md, (POS){ x,y });

			PrintGrid((POS){ x,y }, chess->states[y][x], color, canMove);

			grid = grid * -1;
		}
		grid = grid * -1;
	}
}

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
	for (int _y = 0; _y < GRID_SIZE; _y++)
	{
		for (int _x = 0; _x < GRID_SIZE; _x++)
		{
			gotoxy( (pos.y * GRID_SIZE + _y) + GAP, (pos.x * GRID_SIZE + _x + GAP) * 2);
			if (_x == 1 && _y == 1)
			{
				SetColor(color == Black ? Lgray : Black, canMove != -1? Dgray : color);
				if (sd.player != EMPTY_PLAYER)
				{
					int sdC = sd.player == BLACK_PLAYER ? 1 : 0;
					printf("%c ", chessPiece[sdC][sd.pieceType - 1]);
				}
				else
				{
					SetColor(Black, canMove != -1 ? Dgray : color);
					printf("  ");
				}
			}
			else
			{
				SetColor(Black, color);
				printf("  ");
				//wprintf(L"%c", list[_x + _y * GRID_SIZE]);
			}
		}
	}
}

void PrintState(CHESS* chess) // 체스판 옆에 게임에 대한 추가적인 정보를 표시하는 함수
{
	SetColor(Lgray, Black);
	gotoxy( GAP - 1, (GAP + CHESS_SIZE * GRID_SIZE + 1) * 2);
	printf("┏━━━━━━┓");
	gotoxy( GAP, (GAP + CHESS_SIZE * GRID_SIZE + 1) * 2);
	printf("┃ %s┃", chess->currentPlayer == BLACK_PLAYER ? "BLACK" : "WHITE");
	gotoxy( GAP + 1, (GAP + CHESS_SIZE * GRID_SIZE + 1) * 2);
	printf("┗━━━━━━┛");

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