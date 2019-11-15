#pragma once
#include "GameData.h"

int MainMenu(); //���θ޴� �Լ�, ���� �� -1 ��ȯ

BOOL IsAvailableToMov(CHESS* ch, int y, int x, int curColor); //�ش� ���� y, x�� �̵��� �� �ִ��� ���� ��ȯ
BOOL IsEmpty(CHESS* ch, int y, int x); //�ش� ĭ�� ��������� TRUE

MOVEDATA* GetMoveData(CHESS* ch, const POS pos); //�Ű������� ���޵� ü������ ������� �̵������� ��ġ ���͸� ��ȯ

void Move(CHESS* chess, const POS src, const MOVEDATA desMoveData);
int CalulateState(CHESS* chess, const POS kingPos);
BOOL OtherCanCome(CHESS* chess, const POS pos, int playerColor);
BOOL CheckAround(CHESS* chess, const POS kingPos);
BOOL IsInMap(POS pos);

