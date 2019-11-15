#pragma once
#include "GameData.h"

int MainMenu(); //���θ޴� �Լ�, ���� �� -1 ��ȯ

BOOL IsAvailableToMov(CHESS* ch, int y, int x, int curColor);		//�ش� ���� y, x�� �̵��� �� �ִ��� ���� ��ȯ
BOOL IsEmpty(CHESS* ch, int y, int x);								//�ش� ĭ�� ��������� TRUE
BOOL OtherCanCome(CHESS* chess, const POS pos, int playerColor);	// playerColor�� �ٸ� ���� ���� pos�� �� �� �ִ��� 
BOOL CheckAround(CHESS* chess, const POS kingPos);					// �����̸� �״��� Ȯ��
BOOL IsInMap(POS pos);												// pos�� ü���� �ȿ� �ִ��� Ȯ��

MOVEDATA* GetMoveData(CHESS* ch, const POS pos);					//�Ű������� ���޵� ü������ ������� �̵������� ��ġ ���͸� ��ȯ

void Move(CHESS* chess, const POS src, const MOVEDATA desMoveData); // �⹰�� �����̰� ��������
int CalculateState(CHESS* chess, const POS kingPos);				// Check���� CheckMate���� StaleMate���� �Ǻ���
																	// King�� ������ ���� ���� == 0
																	// Check == 1
																	// ����� ������ ���� �� ���� == 2

