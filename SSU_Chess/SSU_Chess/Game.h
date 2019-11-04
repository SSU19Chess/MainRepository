#pragma once
#include "GameData.h"

int MainMenu(); //메인메뉴 함수, 종료 시 -1 반환

MOVEDATA* GetMoveData(CHESS* ch, const POS pos);
