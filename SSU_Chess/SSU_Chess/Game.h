#pragma once
#include "GameData.h"

int MainMenu(); //���θ޴� �Լ�, ���� �� -1 ��ȯ

BOOL IsAvailableToMov(int y, int x, int curColor); //�ش� ���� y, x�� �̵��� �� �ִ��� ���� ��ȯ
BOOL IsEmpty(CHESS* ch, int y, int x); //�ش� ĭ�� ��������� TRUE

MOVEDATA* GetMoveData(CHESS* ch, const POS pos); //�Ű������� ���޵� ü������ ������� �̵������� ��ġ ���͸� ��ȯ

