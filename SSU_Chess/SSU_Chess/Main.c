#include <stdio.h>
#include <Windows.h>

enum color
{
	Black = 0, Blue, Green,	Cyan,	Red, Magenta, Brown, Lgray, Dgray, Lblue, Lgreen,
	Lcyan, Lred, Lmagenta, Yellow, White
};

void SetColor(WORD text, WORD back);
void GotoXY(int y, int x);
void InitBoard();


int main()
{
	InitBoard();

	return 0;
}

void SetColor(WORD text, WORD back)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text + back * 16);
}

void GotoXY(int y, int x)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void InitBoard()
{
	int flag = -1;

	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (flag == -1) SetColor(0, White);
			else SetColor(0, Black);

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					GotoXY(3 * y + i, 6 * x + 2 * j);
					printf("¡¡");
				}
			}
			flag *= -1;
		}
		flag *= -1;
	}
}