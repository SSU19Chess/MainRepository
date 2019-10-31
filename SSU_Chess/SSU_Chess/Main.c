#include <stdio.h>
#include <Windows.h>

enum color
{
	Black = 0, Blue, Green,	Cyan,	Red, Magenta, Brown, Lgray, Dgray, Lblue, Lgreen,
	Lcyan, Lred, Lmagenta, Yellow, White
};

void SetColor(WORD text, WORD back); //text�� background�� ������ �����Ѵ�.
void GotoXY(int y, int x);			 //gotoxy �Լ�
void InitBoard();					 //ü�����带 ���� ����Ѵ�.


int main()
{
	InitBoard();

	return 0;
}

void SetColor(WORD text, WORD back) //text�� background�� ������ �����Ѵ�.
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text + back * 16);
}

void GotoXY(int y, int x) //gotoxy �Լ�
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void InitBoard() //ü�����带 ���� ����Ѵ�.
{
	int flag = -1; //��� ���� ���� ����Ī ���� -1�� ���ϸ鼭 ����Ī.

	for (int y = 0; y < 8; y++) 
	{
		for (int x = 0; x < 8; x++)
		{
			// flag���� ���� ������ �����Ѵ�.
			if (flag == -1) SetColor(0, White);
			else if (flag == 1) SetColor(0, Black);

			/*====(y, x)ĭ ��º�=========*/
			for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
			{
				GotoXY(3 * y + i, 6 * x + 2 * j);
				printf("��");
			}
			/*===========================*/
			flag *= -1; //��ĭ ����� ������, ������ ����Ī�Ѵ�.
		}
		flag *= -1; //���� ����� ������, ������ ����Ī�Ѵ�.
	}
}