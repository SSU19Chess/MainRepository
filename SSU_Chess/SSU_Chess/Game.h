#pragma once
#include "GameData.h"


/**
	@ �Լ� �̸�: InGame
	@ �Լ� ����: ����ȭ��, ���� ����(Check, CheckMate, StaleMate)�� ȭ�鿡 ����ϰ�
				 ������� �Է�(����Ű �����¿�, Enter)�� �޾� �⹰�� �����̴� �Լ��� ȣ��
				 �Է��� �ް� ���ӿ� ���¿� ���� ������ �����Ѵ�.
	@ ���� �Լ���: Init, PrintBoard, PrintState, Input, PrintResult
	//
**/
int InGame();
/**
	@ �Լ� �̸�: MainMenu
	@ �Լ� ����: ���� ����ȭ���� ȭ�鿡 ������ְ� Ű���� ��, �Ʒ� ����Ű�� ���� '���� ����', '���� ����'�� �����Ѵ�.
				 '���� ����'�� �����ϸ� 0��, '���� ����'�� �����ϸ� -1�� ��ȯ�Ѵ�.
	@ ���� �Լ���: PrtMainMenu, gotoxy
**/
int MainMenu(); //���θ޴� �Լ�, ���� �� -1 ��ȯ
/**
	@ �Լ� �̸�: Input
	@ �Լ� ����: getch �Լ��� �̿��Ͽ� '����Ű �����¿�'�� �޾� ü���ǿ��� Ŀ���� �̵��ϰ�
				 'Enter'�� �Է¹޾Ƽ� �⹰�� �����ϰų�, �⹰�� �����δ�.
	@ �Ķ���� �̸� ����: (chess, lastSelectedPos, currentPos, moveData)
	@ �Ķ���� ����
		@ chess: ü�� ���ӿ� ���� ������ ������ �ִ� ����ü
		@ lastSelectedPos: ������ �⹰�� ��ġ
		@ currentPos: ���� Ŀ���� ��ġ
		@ moveData: ������ �⹰�� �̵��� �� �ִ� ������ ���� ����ü �����͸� ����Ű�� ������
	@ ���� �Լ���: gotoxy, IsInMoveData, Move, GetMoveData
	@ exception ����ó��
	//
**/
void Input(CHESS* chess, POS* lastSelectPos, POS* currentPos, MOVEDATA** moveData);
/**
	@ �Լ� �̸�: Init
	@ �Լ� ����: CHESS ����ü�� �����Ҵ� �޾Ƽ� ü������ �ʱ�ȭ�ϰ� ����ü�� ������ �ʱ�ȭ�ϰ� �ش� ����ü�� ��ȯ�Ѵ�.
	@ ���� �Լ���
	@ exception ����ó��
	// CHESS ����ü �����Ҵ翡 �����ϸ� NULL�� ��ȯ�Ѵ�.
**/
CHESS* Init();
/**
	@ �Լ� �̸�: IsInMoveData
	@ �Լ� ����: pos�� md��� �ȿ� �ִ����� Ȯ���Ͽ� �ִٸ� �ε����� ��ȯ, ������ -1�� ��ȯ�Ѵ�.
	@ �Ķ���� �̸� ���� (md, pos)
	@ �Ķ���� ����
		@ pos: x, y ���� ��� ����ü
		@ md: MOVEDATA ����ü �迭�� ����Ű�� ������
	@ exception ����ó��
	// md�� NULL�� ����Ű�� -1�� ��ȯ
**/
int IsInMoveData(MOVEDATA* md, const POS pos);						// pos�� MOVEDATA ��� �ȿ� �ִ���, pos�� ���� pos�� ���� MOVEDATA�� �ε��� ��ȯ, ������ -1
/**
	@ �Լ� �̸� : IsAvailableToMov
	@ �Լ� ���� : �Ķ���ͷ� ���޵� ���� y, x�� �̵��� �� �ִ��� ���� ��ȯ
	@ �Ķ���� �̸� ���� : (ch, y, x, curColor)
	@ �Ķ���� ���� :
		@ ch : ü�� ���ӿ� ���� ������ ������ �ִ� ����ü
		@ y, x : ���޵� ���� ��ǥ
		@ curColor : ���޵� ���� ������
	@ ���� �Լ��� : IsInMap
	@ exception ����ó��
	//
**/
BOOL IsAvailableToMov(CHESS* ch, int y, int x, int curColor);		// �ش� ���� y, x�� �̵��� �� �ִ��� ���� ��ȯ
/**
	@ �Լ� �̸� : IsEmpty
	@ �Լ� ���� : �ش� ĭ�� ��������� TRUE
	@ �Ķ���� �̸� ���� : ch, y, x
	@ �Ķ���� ���� :
		@ ch : ü�� ���ӿ� ���� ������ ������ �ִ� ����ü
		@ y, x : ���޵� ���� ��ǥ
	@ ���� �Լ��� : IsInMap
	@ exception ����ó��
	//
**/
BOOL IsEmpty(CHESS* ch, int y, int x);								// �ش� ĭ�� ��������� TRUE
/**
	@ �Լ� �̸�: IsInMap
	@ �Լ� ����: pos�� ü���� ���� �ȿ� �ִ��� Ȯ���Ͽ� ���� ���̸� True�� ��ȯ, ���� ���̸� False�� ��ȯ�Ѵ�.
	@ �Ķ���� �̸� ���� (pos)
	@ �Ķ���� ����
		@ pos: x, y ���� ��� ����ü
**/
BOOL IsInMap(POS pos);												// pos�� ü���� �ȿ� �ִ��� Ȯ��

/**
	@ �Լ� �̸�: OtherCanCome
	@ �Լ� ����: playerColor�� �ٸ� ���� ���� pos�� �� �� �ִ����� Ȯ���Ͽ� ����� ��ȯ�Ѵ�.
	@ �Ķ���� �̸� ���� (chess, pos, playerColor)
	@ �Ķ���� ����
		@ chess: ü�� ���ӿ� ���� ������ ������ �ִ� ����ü
		@ pos: playerColor�� �ٸ� ���� ���� �� �� �ִ����� Ȯ���ϱ� ���� ��ġ
		@ playerColor: �˻��� �÷��̾��� ����
		@ isEmpty: �ش� ĭ�� ������ ����ִ����� ǥ���ϴ� ����( ���� �� �� �ִ����� Ȯ���ϱ� ����)
	@ ���� �Լ���: IsInMap
	@ exception ����ó��
	// Ž�� �� ü������ �ε����� �Ѿ ��� for�� ������ break�� �Ǵ�.
**/
BOOL OtherCanCome(CHESS* chess, const POS pos, int playerColor, BOOL isEmpty);	// playerColor�� �ٸ� ���� ���� pos�� �� �� �ִ��� 
/**
	@ �Լ� �̸�: CheckAround
	@ �Լ� ����: King�� ���� 8ĭ���� ������ �� ���ٸ� True, ������ False�� ��ȯ�Ѵ�.
	@ �Ķ���� �̸� ���� (chess, pos, playerColor)
	@ �Ķ���� ����
		@ chess: ü�� ���ӿ� ���� ������ ������ �ִ� ����ü
		@ pos: playerColor�� �ٸ� ���� ���� �� �� �ִ����� Ȯ���ϱ� ���� ��ġ
		@ playerColor: �˻��� �÷��̾��� ����
	@ ���� �Լ���: IsInMap
	@ exception ����ó��
	// Ž�� �� ü������ �ε����� �Ѿ ��� for�� ������ break�� �Ǵ�.
**/
BOOL CheckAround(CHESS* chess, const POS kingPos);					// �����̸� �״��� Ȯ��
/**
	@ �Լ� �̸�: PiecesCanMove
	@ �Լ� ����: curColor�� ���� ���� �÷��̾��� �⹰���� ������ �� �ִ� ���� �ִ��� Ȯ���Ͽ� �����ϸ� True, ������ �� ���ٸ� False�� ��ȯ�Ѵ�.
	@ �Ķ���� �̸� ���� (chess, curColor)
	@ �Ķ���� ����
		@ chess: ü�� ���ӿ� ���� ������ ������ �ִ� ����ü
		@ curColor: Ȯ���� �÷��̾��� ����
	@ ���� �Լ���: GetMoveData
**/
BOOL PiecesCanMove(CHESS* chess, int curColor);						// �⹰���� ������ �� �ִ� ���� �ִ��� Ȯ��
/**
	@ �Լ� �̸�: GetKingPos
	@ �Լ� ����: curColor�� King ��ġ�� ã�Ƽ� ��ȯ�Ѵ�.
	@ �Ķ���� �̸� ���� (chess, curColor)
	@ �Ķ���� ����
		@ chess : ü�� ���ӿ� ���� ������ ������ �ִ� ����ü
		@ src : ���޵� �÷��̾��� ����
**/
POS GetKingPos(CHESS* chess, int curColor);			// curColor�� King��ġ�� ã�Ƽ� ��ȯ�Ѵ�.
/**
	@ �Լ� �̸�: GetMoveData
	@ �Լ� ����: �Ű������� ���޵� ü������ ������� �̵������� ��ġ �����͵��� ��ȯ
	@ �Ķ���� �̸� ���� (ch, pos)
	@ �Ķ���� ����
		@ ch : ü�� ���ӿ� ���� ������ ������ �ִ� ����ü
		@ pos : x, y ���� ��� ����ü
	@ ���� �Լ��� : GetKingPos, CalculateState, Move, IsAvailableToMov, IsEmpty, OtherCanCome
	@ exception ����ó��
	//
**/
MOVEDATA* GetMoveData(CHESS* ch, const POS pos);					// �Ű������� ���޵� ü������ ������� �̵������� ��ġ �����͵��� ��ȯ
/**
	@ �Լ� �̸�: CalculateState
	@ �Լ� ����: ���� chess ����ü�� ������� ������ ���¸� ����Ͽ� ��ȯ�Ѵ�.
				 King�� ����� �⹰���� ������ ���� ���Ⱑ �ƴϸ� 0�� ��ȯ
				 Check�����̸� 1�� ��ȯ
				 King�� ����� �⹰���� ������ ���� �����ε� ���� �� ���ٸ� 2�� ��ȯ�Ѵ�.
	@ �Ķ���� �̸� ���� (chess, kingPosa)
	@ �Ķ���� ����
		@ chess: ü�� ���ӿ� ���� ������ ������ �ִ� ����ü
		@ kingPos: King�� ��ġ
	@ ���� �Լ���: IsInMap, OtherCanCome
**/
int CalculateState(CHESS* chess, const POS kingPos);
/**
	@ �Լ� �̸�: Move
	@ �Լ� ����: src�� �ִ� �⹰�� desMoveData.pos�� �����δ�. �⹰�� Ÿ�Կ� ����, King�� ��� ĳ����, Pawn�� ��� ���Ļ� ���θ� Ȯ���ϰ� �����Ѵ�.
				 �⹰�� �����̰� ������ ����( Check, CheckMate, StaleMate)�� Ȯ���Ѵ�.
				 �� �۾��� ���� �� ���濡�� ���� �ѱ��.
	@ �Ķ���� �̸� ���� (chess, src, desMoveData)
	@ �Ķ���� ����
		@ chess: ü�� ���ӿ� ���� ������ ������ �ִ� ����ü
		@ src: ������ �⹰�� ���� ��ġ
		@ desMoveData: �⹰�� ������ ��ġ�� ���� ������ ���� MOVEDATA ����ü
	@ ���� �Լ���: UpdateEpState, Promotion, GetKingPos, CalculateState, CheckAround, PiecesCanMove
**/
void Move(CHESS* chess, const POS src, const MOVEDATA desMoveData); // �⹰�� �����̰� ��������
/**
	@ �Լ� �̸�: UpdataEpState
	@ �Լ� ����: ���Ļ� ���¸� ������Ʈ �Ѵ�.
				���� ���� ���Ļ��� ������ "���� ���� PAWN��"�� Ž���Ͽ�
				�� ���� ������, ���Ļ��� �Ұ��� �ϵ��� ������ ������Ʈ �Ѵ�.
	@ �Ķ���� �̸� ���� (chess, curColor)
	@ �Ķ���� ����
		@ chess : ü�� ���ӿ� ���� ������ ������ �ִ� ����ü
		@ curColor : ��� ���� ������ ���� �÷��̾��
**/
void UpdateEpState(CHESS* chess, int curColor);						// ���Ļ� ���¸� ������Ʈ �Ѵ�
/**
	@ �Լ� �̸�: Promotion
	@ �Լ� ����: �ش� Pawn�� ���θ���Ѵ�
	@ �Ķ���� �̸� ���� (chess, curPos, curColor)
	@ �Ķ���� ����
		@ chess : ü�� ���ӿ� ���� ������ ������ �ִ� ����ü
		@ curPos : ���θ�� �� Pawn�� ��ġ
		@ curColor : ���θ�� �� Pawn�� ������
	@ ���� �Լ��� : CursorView
**/
void Promotion(CHESS* chess, POS curPos, int curColor);