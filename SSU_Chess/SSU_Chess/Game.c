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


	gotoxy(ypos, 10);
	printf("☞");

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