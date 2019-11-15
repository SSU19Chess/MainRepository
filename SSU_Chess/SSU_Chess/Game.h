#pragma once
#include "GameData.h"

int MainMenu(); //메인메뉴 함수, 종료 시 -1 반환

BOOL IsAvailableToMov(int y, int x, int curColor); //해당 말이 y, x로 이동할 수 있는지 여부 반환
BOOL IsEmpty(CHESS* ch, int y, int x); //해당 칸이 비어있으면 TRUE

MOVEDATA* GetMoveData(CHESS* ch, const POS pos); //매개변수로 전달된 체스판을 기반으로 이동가능한 위치 벡터를 반환

