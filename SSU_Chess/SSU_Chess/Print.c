#include <stdio.h>
#include <Windows.h>
#include "Print.h"

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

void InitBoard() //체스보드를 최초 출력한다.
{
	int flag = -1; //흑과 백의 색깔 스위칭 변수 -1을 곱하면서 스위칭.

	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			// flag값에 따라 색상을 변경한다.
			if (flag == -1) SetColor(0, White);
			else if (flag == 1) SetColor(0, Black);

			/*====(y, x)칸 출력부=========*/
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
				{
					gotoxy(3 * y + i, 6 * x + 2 * j);
					printf("　");
				}
			/*===========================*/
			flag *= -1; //한칸 출력이 끝나면, 색상을 스위칭한다.
		}
		flag *= -1; //한줄 출력이 끝나면, 색상을 스위칭한다.
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