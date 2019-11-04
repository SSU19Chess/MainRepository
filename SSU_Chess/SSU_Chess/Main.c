#include <stdio.h>
#include <Windows.h>
#include "GameData.h"
#include "Game.h"
#include "Print.h"



int main()
{
	int sel = MainMenu();

	if (sel == -1)
		return 0;
	else
		printf("이곳에 게임 시작 입력");
}s