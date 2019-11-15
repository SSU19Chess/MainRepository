#include <stdio.h>
#include <Windows.h>
#include "Print.h"

void SetColor(WORD text, WORD back) //text�� background�� ������ �����Ѵ�.
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text + back * 16);
}

void gotoxy(int y, int x) //gotoxy �Լ�
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void CursorView(char show)//Ŀ�������
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

void InitBoard() //ü�����带 ���� ����Ѵ�.
{
	int flag = -1; //��� ���� ���� ����Ī ���� -1�� ���ϸ鼭 ����Ī.

	for (int y = 0; y < CHESS_SIZE; y++)
	{
		for (int x = 0; x < CHESS_SIZE; x++)
		{
			// flag���� ���� ������ �����Ѵ�.
			if (flag == -1) SetColor(0, White);
			else if (flag == 1) SetColor(0, Black);

			/*====(y, x)ĭ ��º�=========*/
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
				{
					gotoxy(3 * y + i, 6 * x + 2 * j);
					printf("��");
				}
			/*===========================*/
			flag *= -1; //��ĭ ����� ������, ������ ����Ī�Ѵ�.
		}
		flag *= -1; //���� ����� ������, ������ ����Ī�Ѵ�.
	}
}

void PrtMainMenu()
{
	printf("���ߡߡߡߡ��ߡߡߡߡ��ߡ����ߡ�����������������\n");
	printf("���ߡ��������ߡ��������ߡ����ߡ������ߡߡ�������\n");
	printf("���ߡߡߡߡ��ߡߡߡߡ��ߡ����ߡ����ߡ����ߡ�����\n");
	printf("���������ߡ��������ߡ��ߡ����ߡ������������ߡߡ�\n");
	printf("���ߡߡߡߡ��ߡߡߡߡ����ߡߡ�������������������\n\n");
	printf("�����ߡߡߡ��ߡ����ߡ��ߡߡߡߡ��ߡߡߡߡ��ߡߡߡ�\n");
	printf("���ߡ��������ߡ����ߡ��ߡ��������ߡ��������ߡ�����\n");
	printf("���ߡ��������ߡߡߡߡ��ߡߡߡߡ��ߡߡߡߡ��ߡߡߡ�\n");
	printf("���ߡ��������ߡ����ߡ��ߡ��������������ߡ���������\n");
	printf("�����ߡߡߡ��ߡ����ߡ��ߡߡߡߡ��ߡߡߡߡ��ߡߡߡ�\n\n\n");
	printf("�����������������ӽ��ۡ���������������������������\n");
	printf("����������������������\n");


	gotoxy(13, 10);
	printf("��");
}