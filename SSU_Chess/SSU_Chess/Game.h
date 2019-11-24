#pragma once
#include "GameData.h"

int InGame();
int MainMenu(); //���θ޴� �Լ�, ���� �� -1 ��ȯ
void Input(CHESS* chess, POS* lastSelectPos, POS* currentPos, MOVEDATA** moveData);

CHESS* Init();

BOOL IsAvailableToMov(CHESS* ch, int y, int x, int curColor);		// �ش� ���� y, x�� �̵��� �� �ִ��� ���� ��ȯ
BOOL IsEmpty(CHESS* ch, int y, int x);								// �ش� ĭ�� ��������� TRUE
BOOL OtherCanCome(CHESS* chess, const POS pos, int playerColor);	// playerColor�� �ٸ� ���� ���� pos�� �� �� �ִ��� 
BOOL CheckAround(CHESS* chess, const POS kingPos);					// �����̸� �״��� Ȯ��
BOOL IsInMap(POS pos);												// pos�� ü���� �ȿ� �ִ��� Ȯ��
int IsInMoveData(MOVEDATA* md, const POS pos);						// pos�� MOVEDATA ��� �ȿ� �ִ���, pos�� ���� pos�� ���� MOVEDATA�� �ε��� ��ȯ, ������ -1

MOVEDATA* GetMoveData(CHESS* ch, const POS pos);					// �Ű������� ���޵� ü������ ������� �̵������� ��ġ ���͸� ��ȯ
BOOL PiecesCanMove(CHESS* chess, int curColor);						// �⹰���� ������ �� �ִ� ���� �ִ��� Ȯ��

void Move(CHESS* chess, const POS src, const MOVEDATA desMoveData); // �⹰�� �����̰� ��������
int CalculateState(CHESS* chess, const POS kingPos);				// Check���� CheckMate���� StaleMate���� �Ǻ���
																	// King�� ������ ���� ���� == 0
																	// Check == 1
																	// ����� ������ ���� �� ���� == 2

void UpdateEpState(CHESS* chess, int curColor);						// ���Ļ� ���¸� ������Ʈ �Ѵ�
POS GetKingPos(CHESS* chess, int curColor);							// curColor�� King��ġ�� ã�Ƽ� ��ȯ�Ѵ�.