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


	gotoxy(ypos, 10);
	printf("��");

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
			printf("��");
		}
		else if (input == ENTER)
		{
			system("cls");
			return ypos == 14 ? -1 : 0;
		}
	}
}