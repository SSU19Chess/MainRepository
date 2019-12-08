#include <stdio.h>
#include <conio.h>

#include "Game.h"
#include "Print.h"
#include "GameData.h"

// �ʱ� ���� �� ����
const PIECETYPE initPieces[CHESS_SIZE][CHESS_SIZE] = {
	{ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK},
	{PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN},
	{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
	{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
	{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
	{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
	{PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN},
	{ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK}
};

// �ʱ� ���� �� ����
const PIECETYPE stalePieces[CHESS_SIZE][CHESS_SIZE] = {
	{NONE, NONE, NONE, NONE, NONE, KING, NONE, NONE},
	{NONE, NONE, NONE, NONE, NONE, PAWN, NONE, NONE},
	{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
	{NONE, NONE, NONE, NONE, NONE, KING, NONE, NONE},
	{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
	{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
	{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
	{NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE}
};

/**
	@ �Լ� �̸�: InGame
	@ �Լ� ����: ����ȭ��, ���� ����(Check, CheckMate, StaleMate)�� ȭ�鿡 ����ϰ�
				 ������� �Է�(����Ű �����¿�, Enter)�� �޾� �⹰�� �����̴� �Լ��� ȣ��
				 �Է��� �ް� ���ӿ� ���¿� ���� ������ �����Ѵ�.
	@ ���� �Լ���: Init, PrintBoard, PrintState, Input, PrintResult
	//
**/
int InGame()
{
	// ���� ���� -------------------------------
	CHESS* chess = Init();			// ü�� ���� ����
	POS currentPos = { 0,0 };		// ü���ǿ����� ���� ��ġ
	POS lastSelectedPos = { -1, -1 };	// ������ �⹰�� ��ġ, ������ �⹰�� ���ٸ� {-1, -1}
	MOVEDATA* moveData = NULL;		// ������ �⹰�� ������ �� �ִ� ��ġ ������

	// �� ����ȭ��------------------------------
	while (1)
	{
		PrintBoard(chess, moveData);
		PrintState(chess);

		Input(chess, &lastSelectedPos, &currentPos, &moveData);
		if (chess->printInfo.gameState == 2)
		{
			// üũ����Ʈ �߻� ���� ����
			PrintBoard(chess, moveData);
			PrintState(chess);
			PrintResult(chess->currentPlayer * -1);
			return 1;
		}
		else if (chess->printInfo.gameState == 3)
		{
			// �����ϸ���Ʈ �߻� ���� ����
			PrintBoard(chess, moveData);
			PrintState(chess);
			PrintResult(0);
			return 1;
		}
	}
}

/**
	@ �Լ� �̸�: MainMenu
	@ �Լ� ����: ���� ����ȭ���� ȭ�鿡 ������ְ� Ű���� ��, �Ʒ� ����Ű�� ���� '���� ����', '���� ����'�� �����Ѵ�.
				 '���� ����'�� �����ϸ� 0��, '���� ����'�� �����ϸ� -1�� ��ȯ�Ѵ�.
	@ ���� �Լ���: PrtMainMenu, gotoxy
**/
int MainMenu()
{
	// �ʱ� ���� -------------------------------
	CursorView(0);

	// ���� ���� -------------------------------
	int ypos = 20;


	// ����ȭ��---------------------------------
	PrtMainMenu();
	while (1)
	{
		gotoxy(ypos, 35);

		int input = _getch();
		if (input == 224)
		{
			input = _getch();
			switch (input)
			{
			case UP:
				ypos = 20;
				printf("  ");
				break;
			case DOWN:
				ypos = 22;
				printf("  ");
				break;
			}
			gotoxy(ypos, 35);
			SetColor(Red, 0);
			printf("��");
		}
		else if (input == ENTER)
		{
			system("cls");
			CursorView(1);
			return ypos == 14 ? -1 : 0;
		}
	}
}

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
void Input(CHESS* chess, POS* lastSelectedPos, POS* currentPos, MOVEDATA** moveData)
{
	BOOL r = TRUE;
	char c;
	while (r)
	{
		gotoxy(currentPos->y * GRID_SIZE + 1 + GAP, (currentPos->x * GRID_SIZE + 1 + GAP) * 2);
		c = _getch();
		switch (c)
		{
		case RIGHT:      //����Ű ���� �� Ŀ�� x + 1
		{
			currentPos->x++;

			if (currentPos->x >= CHESS_SIZE)   //������ ũ�⸦ ����� ���� ��� x - 1
			{
				currentPos->x--;
			}

			break;
		}

		case LEFT:      //����Ű ���� �� Ŀ�� x - 1
		{
			currentPos->x--;

			if (currentPos->x < 0)      //������ ũ�⸦ ����� ���� ��� x + 1
			{
				currentPos->x++;
			}

			break;
		}

		case UP:      //����Ű ���� �� Ŀ�� y - 1
		{
			currentPos->y--;

			if (currentPos->y < 0)      //������ ũ�⸦ ����� ���� ��� y + 1
				currentPos->y++;

			break;
		}

		case DOWN:      //����Ű �Ʒ��� �� Ŀ�� y + 1
		{
			currentPos->y++;

			if (currentPos->y >= CHESS_SIZE)   //������ ũ�⸦ ����� ���� ��� y - 1
				currentPos->y--;

			break;
		}

		case ENTER:      //����Ű �Է� ��
		{	
			int index = IsInMoveData(*moveData, *currentPos); // moveData�� NULL�̰ų�, moveData �迭 �ȿ� currentPos�� ���� pos�� ���� ���� ���ٸ� -1, ������ �ε��� ��ȯ
			if (index != -1) // moveData �ȿ� currentPos�� �ִٸ�
			{
				Move(chess, *lastSelectedPos, *(*moveData + index)); // �⹰�� �����̰�
				*lastSelectedPos = (POS){ -1,-1 };	// ������ �⹰�� ��ġ�� �����ϴ� ������ �ʱ�ȭ �Ѵ�.

				if (*moveData != NULL)
				{
					free(*moveData); // �޸� ����
					*moveData = NULL;
				}
			}			
			else
			{
				if (chess->states[currentPos->y][currentPos->x].pieceType != NONE)					// ������ ��ġ�� ������� �ʰ�
				{
					if (chess->states[currentPos->y][currentPos->x].player == chess->currentPlayer) // ������ ��ġ�� �⹰�� ���� ���� ���� �÷��̾��� ���� ���ٸ�
					{
						memcpy(lastSelectedPos, currentPos, sizeof(POS));	// ������ ��ġ�� �����ϰ�
						*moveData = GetMoveData(chess, *currentPos);		// ������ ��ġ�� �ִ� �⹰�� �̵������� ��θ� ��´�.

						if (_msize(*moveData) / sizeof(MOVEDATA) == 0)	// ���� �⹰�� �̵������� ��ΰ� ���ٸ�
						{
							free(*moveData);	// �Ҵ���� �޸𸮸� �������ְ�
							*moveData = NULL;	// �ƹ��͵� ����Ű�� ���� �ʴ´�.
						}
					}
				}
				else
				{
					if (*moveData != NULL)
					{
						free(*moveData); // �޸� ����
						*moveData = NULL;
					}
				}
			}
			r = FALSE;      //r�� ���� 0�� ���� (���ѷ��� Ż��)
			break;
		}
		}
	}
}


/**
	@ �Լ� �̸�: Init
	@ �Լ� ����: CHESS ����ü�� �����Ҵ� �޾Ƽ� ü������ �ʱ�ȭ�ϰ� ����ü�� ������ �ʱ�ȭ�ϰ� �ش� ����ü�� ��ȯ�Ѵ�.
	@ ���� �Լ���
	@ exception ����ó��
	// CHESS ����ü �����Ҵ翡 �����ϸ� NULL�� ��ȯ�Ѵ�.
**/
CHESS* Init()
{
	CHESS* chess = (CHESS*)malloc(sizeof(CHESS));
	if (chess == NULL)
		return NULL;

	for (int y = 0; y < CHESS_SIZE; y++)
	{
		for (int x = 0; x < CHESS_SIZE; x++)
		{
			int color = EMPTY_PLAYER;
			if (y <= 1)
				color = BLACK_PLAYER;
			else if (y >= 6)
				color = WHITE_PLAYER;

			chess->states[y][x] = (STATEDATA){ initPieces[y][x], color, 0 };
		}
	}
	chess->currentPlayer = WHITE_PLAYER;

	chess->printInfo.gameState = 0;
	for (int i = 0; i < 2; i++)
	{
		chess->printInfo.diedPieceCnt[i] = 0;
		for (int j = 0; j < 20; j++)
		{
			chess->printInfo.diedPiece[i][j] = NONE;
		}
	}
	return chess;
}

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
int IsInMoveData(const MOVEDATA* md, const POS pos)
{
	if (md == NULL)
		return -1;

	size_t size = _msize(md) / sizeof(MOVEDATA);
	for (int i = 0; i < size; i++)
	{
		if (md[i].pos.x == pos.x && md[i].pos.y == pos.y)
			return i;
	}
	return -1;
}

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
BOOL IsAvailableToMov(CHESS* ch, int y, int x, int curColor) 
{
	if (ch->states[y][x].player == curColor) //�ش� ĭ�� ������ �⹰�� ������ FALSE
		return FALSE;

	if(!IsInMap((POS){x, y}))
		return FALSE;
	
	return TRUE;
}

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
BOOL IsEmpty(CHESS* ch, int y, int x)
{
	if (!IsInMap((POS) { x, y }))
		return FALSE;

	if (ch->states[y][x].pieceType == NONE)
		return TRUE;
	else
		return FALSE;
}

/**
	@ �Լ� �̸�: IsInMap
	@ �Լ� ����: pos�� ü���� ���� �ȿ� �ִ��� Ȯ���Ͽ� ���� ���̸� True�� ��ȯ, ���� ���̸� False�� ��ȯ�Ѵ�.
	@ �Ķ���� �̸� ���� (pos)
	@ �Ķ���� ����
		@ pos: x, y ���� ��� ����ü
**/
BOOL IsInMap(POS pos)
{
	if (pos.x < 0 || pos.x >= CHESS_SIZE || pos.y < 0 || pos.y >= CHESS_SIZE)
		return FALSE;
	return TRUE;
}

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
BOOL OtherCanCome(CHESS* chess, const POS pos, int playerColor, BOOL isEmpty)
{
	int posColor = chess->states[pos.y][pos.x].player; // pos ��ġ�� ����
	int dir[][2] = {
		{1,0}, {0,1}, {1,1}, {1, -1}
	};

	// ����Ʈ�� ������ ������ pos�� �� �� �ִ���
	for (int d = 0; d < 4; d++)
	{
		for (int s = -1; s <= 1; s += 2)
		{
			for (int i = 1; i < CHESS_SIZE - 1; i++)
			{
				POS _pos = { pos.x + i * dir[d][0] * s, pos.y + i * dir[d][1] * s };
				if (!IsInMap(_pos))
					break;

				// playerColor�� ���� ������ �⹰�̸�, break;
				if (chess->states[_pos.y][_pos.x].player == playerColor)
					break;

				if (d == 0 || d == 1)
				{
					if (chess->states[_pos.y][_pos.x].pieceType == ROOK ||
						chess->states[_pos.y][_pos.x].pieceType == QUEEN)
					{
						return TRUE;
					}
				}
				else if (d == 2 || d == 3)
				{
					if (chess->states[_pos.y][_pos.x].pieceType == BISHOP ||
						chess->states[_pos.y][_pos.x].pieceType == QUEEN)
					{
						return TRUE;
					}
					else if (chess->states[_pos.y][_pos.x].pieceType == PAWN)
					{
						if (i == 1 && !isEmpty)
						{
							if (playerColor == BLACK_PLAYER)
							{
								// �������� �� ����� ���� ����̹Ƿ� 
								// ��밡 ���� �밢�� �������� �����ϹǷ�
								// �˻����ϴ� �������� �Ʒ��� �밢���� �˻�
								if ((d == 2 && s == 1) || (d == 3 && s == -1))
									return TRUE;
							}
							else
							{
								if ((d == 2 && s == -1) || (d == 3 && s == 1))
									return TRUE;
							}
						}
					}
				}

				if (chess->states[_pos.y][_pos.x].player != NONE)
					break;
			}
		}
	}

	// ����Ʈ�� King�� ������ �� �ִ���
	int knightMoves[][2] = {
		{2, 1}, {1, 2}
	};
	for (int y = -1; y <= 1; y += 2)
	{
		for (int x = -1; x <= 1; x += 2)
		{
			for (int k = 0; k < 2; k++)
			{
				POS _checkPos = { pos.x + knightMoves[k][0] * x, pos.y + knightMoves[k][1] * y };
				if (!IsInMap(_checkPos))
					break;

				if (chess->states[_checkPos.y][_checkPos.x].pieceType == KNIGHT &&
					chess->states[_checkPos.y][_checkPos.x].player != playerColor)
				{
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

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
BOOL CheckAround(CHESS* chess, const POS kingPos) 
{
	BOOL aroundOnlySameColor = TRUE; // �ֺ� �⹰�� ������ �� kingPos�� ���� ������
	BOOL aroundIsFull = TRUE; // �ֺ��� ������ �� �ִ� ��ġ�� �ִ���, �� pieceType�� NONE�� ��ġ�� �ִ���

	for (int y = -1; y <= 1; y++)
	{
		for (int x = -1; x <= 1; x++)
		{
			// kingPos�� Ȯ���� �ʿ� ����
			if (x == 0 && y == 0)
				continue;
			POS _kingPos = { kingPos.x + x, kingPos.y + y };
			// �� �۵� Ȯ���� �ʿ� ����
			if (!IsInMap(_kingPos))
				continue;

			// ������ ����� ŷ�� �ִ��� Ȯ��
			BOOL canMoveHere = TRUE;
			for (int j = -1; j <= 1; j++)
			{
				for (int i = -1; i <= 1; i++)
				{
					if (i == 0 && j == 0)
						break;
					POS __kingPos = { kingPos.x + x + i, kingPos.y + y + j};
					// �� �۵� Ȯ���� �ʿ� ����
					if (!IsInMap(__kingPos))
						continue;

					if (chess->states[__kingPos.y][__kingPos.x].pieceType == KING &&
						chess->states[__kingPos.y][__kingPos.x].player != chess->states[kingPos.y][kingPos.x].player)
					{
						canMoveHere = FALSE;
						break;
					}
				}
				if (!canMoveHere)
					break;
			}
			if (!canMoveHere)
				continue;


			if (chess->states[_kingPos.y][_kingPos.x].pieceType != NONE)
			{
				// �ٸ� ���� �⹰�� �ִٸ� aroundOnlySameColor�� False�� 
				if (chess->states[_kingPos.y][_kingPos.x].player != chess->states[kingPos.y][kingPos.x].player)
				{
					if (!OtherCanCome(chess, _kingPos, chess->states[kingPos.y][kingPos.x].player, FALSE))
						return FALSE;
					aroundOnlySameColor = FALSE;
				}
				continue;
			}

			aroundIsFull = FALSE; // �� if���� ���� �ֺ��� �� ������ ������ �� �� ����

			if (!OtherCanCome(chess, _kingPos, chess->states[kingPos.y][kingPos.x].player, FALSE))
				return FALSE;
		}
	}
	// �ֺ��� �� ������ ������ �� ���� ���̸�, ������ �� ��� ��� ����
	if (aroundOnlySameColor && aroundIsFull)
		return FALSE;
	return TRUE;

}

/**
	@ �Լ� �̸�: PiecesCanMove
	@ �Լ� ����: curColor�� ���� ���� �÷��̾��� �⹰���� ������ �� �ִ� ���� �ִ��� Ȯ���Ͽ� �����ϸ� True, ������ �� ���ٸ� False�� ��ȯ�Ѵ�.
	@ �Ķ���� �̸� ���� (chess, curColor)
	@ �Ķ���� ����
		@ chess: ü�� ���ӿ� ���� ������ ������ �ִ� ����ü
		@ curColor: Ȯ���� �÷��̾��� ����
	@ ���� �Լ���: GetMoveData
**/
BOOL PiecesCanMove(CHESS* chess, int curColor)
{
	for (int y = 0; y < CHESS_SIZE; y++)
	{
		for (int x = 0; x < CHESS_SIZE; x++)
		{
			if (chess->states[y][x].player == curColor)
			{
				MOVEDATA* m = GetMoveData(chess, (POS) { x, y });
				size_t t = _msize(m) / sizeof(MOVEDATA);
				if (t != 0)
					return TRUE;
			}
		}
	}
	return FALSE;
}

/**
	@ �Լ� �̸�: GetKingPos
	@ �Լ� ����: curColor�� King ��ġ�� ã�Ƽ� ��ȯ�Ѵ�.
	@ �Ķ���� �̸� ���� (chess, curColor)
	@ �Ķ���� ����
		@ chess : ü�� ���ӿ� ���� ������ ������ �ִ� ����ü
		@ src : ���޵� �÷��̾��� ����
**/
POS GetKingPos(CHESS* chess, int curColor)
{
	POS ret;

	//���� Ž������ KING�� ��ġ�� ã�´�.
	for (int y = 0; y < CHESS_SIZE; y++)
	{
		for (int x = 0; x < CHESS_SIZE; x++)
		{
			if (chess->states[y][x].player == curColor &&
				chess->states[y][x].pieceType == KING)
			{
				ret = (POS){ x, y };
				return ret;
			}
		}
	}

	return (POS) { -1, -1 }; //ERROR!
}

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
MOVEDATA* GetMoveData(CHESS* ch, const POS pos)
{
	MOVEDATA* mvData; 
	MOVEDATA* chkedData; //üũ �Ǿ�������, ��ȯ��

	const STATEDATA curState = ch->states[pos.y][pos.x];
	const int curColor = curState.player;			 //���� �÷��̾��� color
	const int oppoColor = curColor * -1;			 //��� �÷��̾��� color

	int cnt = 0;									 //mvData�� ���� ��, �̵������� ��ġ�� ����
	mvData = (MOVEDATA*)malloc(sizeof(MOVEDATA) * cnt); //���̰� 0�� �� ���� �Ҵ�

	POS curKingPos = GetKingPos(ch, curColor);

	/* Ư�� PIECETYPE�� MOVEDATA */
	const int knightMovDataY[8] = { 2, 1, -1, -2, -2, -1, 1, 2 }; //KNIGHT�� �̵� ������
	const int knightMovDataX[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };

	const int bishopMovDataY[4] = { 1, 1, -1, -1 };		   //BISHOP�� �̵� ������
	const int bishopMovDataX[4] = { -1, 1, -1, 1 };

	const int rookMovDataY[4] = { 1, -1, 0, 0 };			//ROOK�� �̵� ������
	const int rookMovDataX[4] = { 0, 0, 1, -1 };

	const int kingMovDataY[8] = {  1, 1, 1, 0, -1, -1, -1,  0 }; //KING�� �̵� ������
	const int kingMovDataX[8] = { -1, 0, 1, 1,  1,  0, -1, -1 }; 

	
	/* Castling�� ���� ���� */
	
	switch (curState.pieceType)
	{
	case PAWN: //���� ���õ� �⹰�� PAWN�� ���

		for (int i = 1; ; i *= -1) //���Ļ�
		{
			if (ch->states[pos.y][pos.x + i].epState == TRUE)
			{
				mvData = (MOVEDATA*)realloc(mvData, sizeof(MOVEDATA) * (++cnt));
				mvData[cnt - 1].pos.y = pos.y + curColor;
				mvData[cnt - 1].pos.x = pos.x + i;
				mvData[cnt - 1].isEP = TRUE; //���Ļ����� ���� �� �ִ� ����
			}
			
			if (i == -1) break;
		}

		for (int i = 1; ; i *= -1) //�밢���� �� �⹰�� �ִ��� �Ǵ�
		{
			if (ch->states[pos.y + curColor][pos.x + i].player == oppoColor) 
			{
				if (IsAvailableToMov(ch, pos.y + curColor, pos.x + i, curColor))
				{
					mvData = (MOVEDATA*)realloc(mvData, sizeof(MOVEDATA) * (++cnt));
					mvData[cnt - 1].pos.y = pos.y + curColor;
					mvData[cnt - 1].pos.x = pos.x + i;
				}
			}
			if (i == -1) break;
		}
		if (curState.moveCnt == 0) //Pawn�� �ѹ��� �������� �ʾ��� ���
		{
			for (int i = 0; i <= 1; i++)
			{
				POS nextPos = { pos.x, pos.y + (curColor * (i + 1)) };

				if (!IsEmpty(ch, nextPos.y, nextPos.x)) break;
				if (!IsAvailableToMov(ch, nextPos.y, nextPos.x, curColor)) break;

				mvData = (MOVEDATA*)realloc(mvData, sizeof(MOVEDATA) * (++cnt));

				mvData[cnt - 1].isCastling = FALSE;
				mvData[cnt - 1].pos.y = nextPos.y;
				mvData[cnt - 1].pos.x = nextPos.x;
			}
		}
		else //Pawn�� �ѹ� �̻� �������� ���
		{
			if (!IsEmpty(ch, pos.y + curColor, pos.x)) break;

			mvData = (MOVEDATA*)realloc(mvData, sizeof(MOVEDATA) * (++cnt));
			mvData[cnt - 1].pos.y = pos.y + curColor;
			mvData[cnt - 1].pos.x = pos.x;
		}
		break;

	case KNIGHT: //���� ���õ� �⹰�� KNIGHT�� ���

		for (int i = 0; i < 8; i++)
		{
			POS nextPos = { pos.x + knightMovDataX[i], pos.y + knightMovDataY[i] };
			if (IsAvailableToMov(ch, nextPos.y, nextPos.x, curColor))
			{
				mvData = (MOVEDATA*)realloc(mvData, sizeof(MOVEDATA) * (++cnt));
				mvData[cnt - 1].pos.y = nextPos.y;
				mvData[cnt - 1].pos.x = nextPos.x;
			}
		}
		break;

	case QUEEN: //���� ���õ� �⹰�� QUEEN�� ��� (ROOK�� BISHOP�� case�� ����)
	case BISHOP: //���� ���õ� �⹰�� BISHOP�� ���

		for (int i = 0; i < 4; i++)
		{
			int plus = 1;
			POS nextPos = { pos.x + plus * bishopMovDataX[i], pos.y + plus * bishopMovDataY[i] };

			while (IsAvailableToMov(ch, nextPos.y, nextPos.x, curColor))
			{
				mvData = (MOVEDATA*)realloc(mvData, sizeof(MOVEDATA) * (++cnt));
				mvData[cnt - 1].pos.y = nextPos.y;
				mvData[cnt - 1].pos.x = nextPos.x;

				if (ch->states[nextPos.y][nextPos.x].player == oppoColor) break;

				nextPos.y = pos.y + plus * bishopMovDataY[i];
				nextPos.x = pos.x + plus * bishopMovDataX[i];
				plus++;
			}
		}
		if (curState.pieceType != QUEEN) //QUEEN �̸�, ROOK�� case�� ����.
			break;
				
	           //���� ���õ� �⹰�� QUEEN�� ���
	case ROOK: //���� ���õ� �⹰�� ROOK�� ���

		for (int i = 0; i < 4; i++)
		{
			int plus = 1;
			POS nextPos = { pos.x + plus * rookMovDataX[i], pos.y + plus * rookMovDataY[i] };

			while (IsAvailableToMov(ch, nextPos.y, nextPos.x, curColor))
			{
				mvData = (MOVEDATA*)realloc(mvData, sizeof(MOVEDATA) * (++cnt));
				mvData[cnt - 1].pos.x = nextPos.x;
				mvData[cnt - 1].pos.y = nextPos.y;

				if (ch->states[nextPos.y][nextPos.x].player == oppoColor) break;

				nextPos.x = pos.x + plus * rookMovDataX[i];
				nextPos.y = pos.y + plus * rookMovDataY[i];
				plus++;
			}
		}
		break;

	case KING: //���� ���õ� �⹰�� KING�� ���
		
		//�Ϲ����� �̵�
		for (int i = 0; i < 8; i++)
		{
			POS nextPos = { pos.x + kingMovDataX[i], pos.y + kingMovDataY[i] };

			// ������ ����� ŷ�� �ִ��� Ȯ��
			BOOL canMoveHere = TRUE;
			for (int j = -1; j <= 1; j++)
			{
				for (int i = -1; i <= 1; i++)
				{
					if (i == 0 && j == 0)
						break;
					POS __kingPos = { nextPos.x + i, nextPos.y + j };
					// �� �۵� Ȯ���� �ʿ� ����
					if (!IsInMap(__kingPos))
						continue;

					if (ch->states[__kingPos.y][__kingPos.x].pieceType == KING &&
						ch->states[__kingPos.y][__kingPos.x].player != curColor)
					{
						canMoveHere = FALSE;
						break;
					}
				}
				if (!canMoveHere)
					break;
			}


			if (IsAvailableToMov(ch, nextPos.y, nextPos.x, curColor) && !OtherCanCome(ch, (POS){nextPos.x, nextPos.y}, curColor, FALSE) && canMoveHere)
			{
				mvData = (MOVEDATA*)realloc(mvData, sizeof(MOVEDATA) * (++cnt));
				mvData[cnt - 1].pos.y = nextPos.y;
				mvData[cnt - 1].pos.x = nextPos.x;
			}
		}
		
		//ĳ����
		//ŷ�� ������ ���θ� �켱 �Ǵ�
		if (ch->states[curColor == WHITE_PLAYER ? 7 : 0][curColor == WHITE_PLAYER ? 4 : 4].pieceType == KING &&
			ch->states[curColor == WHITE_PLAYER ? 7 : 0][curColor == WHITE_PLAYER ? 4 : 4].moveCnt == 0)
		{
			POS kingPos =  curColor == WHITE_PLAYER ? (POS) {4, 7} : (POS) {4, 0} ;
			POS rookPosK =  curColor == WHITE_PLAYER ? (POS) {7, 7} : (POS) {7, 0} ;
			POS rookPosQ =  curColor == WHITE_PLAYER ? (POS) {0, 7} : (POS) {0, 0} ;

			//���� ŷ�� üũ�Ǿ��ִ��� �Ǵ�
			if (CalculateState(ch, (POS) { kingPos.x, kingPos.y }) == 1)
				break;

			//ŷ���̵� ĳ����
			//���� ������ ���� �Ǵ�
			if (ch->states[rookPosK.y][rookPosK.x].pieceType == ROOK &&
				ch->states[rookPosK.y][rookPosK.x].moveCnt == 0)
			{
				int flag = FALSE; //flag�� true? : ĳ���� ���� ���� X

				for (int i = 1; i <= 2; i++) //ŷ�� �� ������ ��ĭ�� ���� �⹰�� �̵��� �������� ���� �Ǵ�
				{
					POS nextPos = { kingPos.x + i, kingPos.y };

					if (!IsEmpty(ch, nextPos.y, nextPos.x) ||
						OtherCanCome(ch, (POS) {nextPos.x, nextPos.y}, curColor,FALSE))
						flag = TRUE;
				}
				if (!flag) 
				{
					mvData = (MOVEDATA*)realloc(mvData, sizeof(MOVEDATA) * (++cnt));
					mvData[cnt - 1].pos.y = kingPos.y;
					mvData[cnt - 1].pos.x = kingPos.x + 2;
					mvData[cnt - 1].isCastling = 1;
				}
			}
			
			//�����̵� ĳ����
			//���� ������ ���� �Ǵ�
			if (ch->states[rookPosQ.y][rookPosQ.x].pieceType == ROOK &&
				ch->states[rookPosQ.y][rookPosQ.x].moveCnt == 0)
			{
				int flag = FALSE; //flag�� true? : ĳ���� ���� ���� X

				for (int i = 1; i <= 2; i++) //ŷ�� �� ������ ��ĭ�� ���� �⹰�� �̵��� �������� ���� �Ǵ�
				{
					POS nextPos = { kingPos.x - i, kingPos.y };

					if (!IsEmpty(ch, nextPos.y, nextPos.x) || 
						OtherCanCome(ch, (POS) { nextPos.x, nextPos.y }, curColor, FALSE))
						flag = TRUE;
				}
				if (!flag)
				{
					mvData = (MOVEDATA*)realloc(mvData, sizeof(MOVEDATA) * (++cnt));
					mvData[cnt - 1].pos.y = kingPos.y;
					mvData[cnt - 1].pos.x = kingPos.x - 2;
					mvData[cnt - 1].isCastling = 2;
				}
			}
		}
		

		break;
	}

	//ŷ�� üũ�ΰ�� mvData �˻�
	//mvData���� üũ�� ����� �ϴ� ������ chkedData�� �־� ��ȯ
	if (ch->printInfo.gameState == 1)
	{
		CHESS ch2;
		ch2 = *ch;

		chkedData = (MOVEDATA*)malloc(sizeof(MOVEDATA) * cnt);
		cnt = 0;

		for (size_t idx = 0; idx < _msize(mvData) / sizeof(MOVEDATA); idx++)
		{
			Move(&ch2, pos, *(mvData+idx));
			curKingPos = GetKingPos(&ch2, curColor);
			if (CalculateState(&ch2, curKingPos) == 0)
			{
				chkedData = (MOVEDATA*)realloc(chkedData, sizeof(MOVEDATA) * (++cnt));
				chkedData[cnt - 1].pos = mvData[idx].pos;
				chkedData[cnt - 1].isCastling = mvData[idx].isCastling;
				chkedData[cnt - 1].isEP = mvData[idx].isEP;
			}
			ch2 = *ch;
		}

		return chkedData;
	}
	else
		return mvData;
}

// King�� ������ ���� ���� == 0
// Check == 1
// ����� ������ ���� �� ���� == 2
// Check���� CheckMate���� StaleMate���� �Ǻ���
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
int CalculateState(CHESS* chess, const POS kingPos) 
{
	int kingColor = chess->states[kingPos.y][kingPos.x].player; // King�� ����
	int dir[][2] = {
		{1,0}, {0,1}, {1,1}, {1, -1}
	}; // ������ ��Ÿ��, ���� �¿� �밢��

	BOOL existEnemy = FALSE;// kingPos�� ������ �� �ִ� ���� �ִ���

	// ����Ʈ�� ������ ������ King�� ������ �� �ִ���
	for (int d = 0; d < 4; d++)
	{
		for (int s = -1; s <= 1; s += 2) // kingPos�� �߽����� ������ ��Ÿ��
		{
			for (int i = 1; i < CHESS_SIZE - 1; i++) // kingPos�� �߽����� �ֺ� 1ĭ���� �� Ž����
			{
				POS _checkPos = { kingPos.x + i * dir[d][0] * s, kingPos.y + i * dir[d][1] * s }; 

				// �� ���̸� break;
				if (!IsInMap(_checkPos))
					break;

				// King�ϰ� ���� ������ �⹰�� ������ �ٸ� ��������, break;
				if (chess->states[_checkPos.y][_checkPos.x].player == kingColor)
					break;


				// ���ϳ� �¿�� Ž���� �� 
				if (d == 0 || d == 1)
				{
					// ����� ROOK�̳� QUEEN �⹰�� ������
					if (chess->states[_checkPos.y][_checkPos.x].pieceType == ROOK ||
						chess->states[_checkPos.y][_checkPos.x].pieceType == QUEEN)
					{
						existEnemy = TRUE; // ������� ������ �� �ִٰ� ����
						BOOL flag = FALSE; // ��� �⹰�� �����ϰų�, �� ���ݰ�θ� ���� �� �ִ���
						int color = kingColor == BLACK_PLAYER ? WHITE_PLAYER : BLACK_PLAYER;
						for (int j = i; j >= 1; j--)
						{
							POS _pos = { kingPos.x + j * dir[d][0] * s, kingPos.y + j * dir[d][1] * s };
							if (OtherCanCome(chess, _pos, color,TRUE))
							{
								flag = TRUE;
								break;
							}
						}
						if (flag)
							return 1;
					}
				}
				// �밢������ Ž���� ��
				else if (d == 2 || d == 3)
				{
					// ����� BISHOP�̳� QUEEN �⹰�� ������
					if (chess->states[_checkPos.y][_checkPos.x].pieceType == BISHOP ||
						chess->states[_checkPos.y][_checkPos.x].pieceType == QUEEN)
					{
						existEnemy = TRUE; // ������� ������ �� �ִٰ� ����
						BOOL flag = FALSE; // ��� �⹰�� �����ϰų�, �� ���ݰ�θ� ���� �� �ִ���
						int color = kingColor == BLACK_PLAYER ? WHITE_PLAYER : BLACK_PLAYER;
						for (int j = i; j >= 1; j--)
						{
							POS _pos = { kingPos.x + j * dir[d][0] * s, kingPos.y + j * dir[d][1] * s };
							if (OtherCanCome(chess, _pos, color, TRUE))
							{
								flag = TRUE;
								break;
							}
						}
						if (flag)
							return 1;
					}
					// ����� PAWN�� ������
					else if (chess->states[_checkPos.y][_checkPos.x].pieceType == PAWN)
					{
						// Ž���ϰ� �ִ� ĭ�� kingPos�κ��� 1ĭ������ ��ġ�� ��
						if (i == 1)
						{
							// ������ �÷��̾��� ��
							if (chess->currentPlayer == BLACK_PLAYER)
							{
								if ((d == 2 && s == -1) || (d == 3 && s == -1))
								{
									existEnemy = TRUE;
									int color = kingColor == BLACK_PLAYER ? WHITE_PLAYER : BLACK_PLAYER;
									if (OtherCanCome(chess, _checkPos, color, TRUE))
										return 1;
								}
							}
							else
							{
								if ((d == 2 && s == 1) || (d == 3 && s == 1))
								{
									existEnemy = TRUE;
									int color = kingColor == BLACK_PLAYER ? WHITE_PLAYER : BLACK_PLAYER;
									if (OtherCanCome(chess, _checkPos, color, TRUE))
										return 1;
								}
							}
						}
					}
				}
				// Ž����ġ�� �� ������ �ƴϸ�, �� �̻� Ž���� �� �����Ƿ� break;
				if (chess->states[_checkPos.y][_checkPos.x].player != NONE)
					break;
			}
		}
	}

	// ����Ʈ�� King�� ������ �� �ִ���
	int knightMoves[][2] = {
		{2, 1}, {1, 2}
	};

	for (int y = -1; y <= 1; y += 2)
	{
		for (int x = -1; x <= 1; x += 2)
		{
			for (int k = 0; k < 2; k++)
			{
				POS _checkPos = { kingPos.x + knightMoves[k][0] * x, kingPos.y + knightMoves[k][1] * y };
				if (!IsInMap(_checkPos))
					break;
				if (chess->states[_checkPos.y][_checkPos.x].pieceType == KNIGHT &&
					chess->states[_checkPos.y][_checkPos.x].player != kingColor)
				{
					existEnemy = TRUE;
					int color = kingColor == BLACK_PLAYER ? WHITE_PLAYER : BLACK_PLAYER;
					if (OtherCanCome(chess, _checkPos, color, TRUE))
						return 1;
				}
			}
		}
	}
	if (existEnemy)
		return 2;
	return 0;
}

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
void Move(CHESS* chess, const POS src, const MOVEDATA desMoveData)
{
	UpdateEpState(chess, chess->states[src.y][src.x].player); //���Ļ� �� ����

	int curPlayer = chess->states[src.y][src.x].player;

	// ������ ��ġ�� ��� ���� �ִ� ���
	if (chess->states[desMoveData.pos.y][desMoveData.pos.x].pieceType != NONE)
	{
		int playerIndex = chess->states[src.y][src.x].player == BLACK_PLAYER ? 1 : 0;
		chess->printInfo.diedPiece[playerIndex][chess->printInfo.diedPieceCnt[playerIndex]++] = chess->states[desMoveData.pos.y][desMoveData.pos.x].pieceType;
	}

	switch (chess->states[src.y][src.x].pieceType)
	{
	case KING: // King�� ��� ĳ���� Ȯ��
		if (desMoveData.isCastling == 1 || desMoveData.isCastling == 2)
		{
			//King �̵�
			chess->states[src.y][src.x].moveCnt++;
			chess->states[desMoveData.pos.y][desMoveData.pos.x] = chess->states[src.y][src.x];
			chess->states[src.y][src.x] = (STATEDATA){ .pieceType = NONE, .player = EMPTY_PLAYER, .moveCnt = 0 }; // �����̱� ���� ��ġ�� NONE����

			//Rook �̵�
			POS rookPos = { 0, 0 };
			POS rookDesPos = { 0, 0 };

			if (desMoveData.isCastling == 1) //ŷ ���̵� ĳ������ ���
			{
				rookPos = (curPlayer == WHITE_PLAYER ? (POS) { 7, 7 } : (POS) { 7, 0 });
				rookDesPos = (curPlayer == WHITE_PLAYER ? (POS) { 5, 7 } : (POS) { 5, 0 });
			}
			else if (desMoveData.isCastling == 2) // �� ���̵� ĳ������ ���
			{
				rookPos = (curPlayer == WHITE_PLAYER ? (POS) { 0, 7 } : (POS) { 0, 0 });
				rookDesPos = (curPlayer == WHITE_PLAYER ? (POS) { 3, 7 } : (POS) { 3, 0 });
			}

			chess->states[rookPos.y][rookPos.x].moveCnt++;
			chess->states[rookDesPos.y][rookDesPos.x] = chess->states[rookPos.y][rookPos.x];
			chess->states[rookPos.y][rookPos.x] = (STATEDATA){ .pieceType = NONE, .player = EMPTY_PLAYER, .moveCnt = 0 }; // �����̱� ���� ��ġ�� NONE����			

			break;
		}


	case PAWN:
		chess->states[src.y][src.x].moveCnt++;
		chess->states[desMoveData.pos.y][desMoveData.pos.x] = chess->states[src.y][src.x]; // �⹰�� �����̰�

		//���Ļ��� ���Ҽ� �ִ��� �Ǵ��ϴ� ������ ������Ʈ
		if (chess->states[src.y][src.x].moveCnt == 1 &&								 //���ʷ� �����̴� ����̰�,
			desMoveData.pos.y == (chess->states[src.y][src.x].player == WHITE_PLAYER ? 4 : 3)) //2ĭ ������ �����ϴ� ���
			chess->states[desMoveData.pos.y][desMoveData.pos.x].epState = TRUE;

		chess->states[src.y][src.x] = (STATEDATA){ .pieceType = NONE, .player = EMPTY_PLAYER, .moveCnt = 0 }; // �����̱� ���� ��ġ�� NONE����

		//���Ļ� ������ �����Ͽ� ���� ���� �ڷ� �̵��� ���, 
		if (desMoveData.isEP == TRUE) 
		{
			int playerIndex = chess->states[src.y][src.x].player == BLACK_PLAYER ? 1 : 0;
			chess->printInfo.diedPiece[playerIndex][chess->printInfo.diedPieceCnt[playerIndex]++] = chess->states[desMoveData.pos.y - curPlayer][desMoveData.pos.x].pieceType;
			//������ �⹰�� ����.
			chess->states[desMoveData.pos.y - curPlayer][desMoveData.pos.x] = (STATEDATA){ .pieceType = NONE, .player = EMPTY_PLAYER, .moveCnt = 0 }; //���Ļ� ���� ���� ��ġ�� NONE����			
		}

		//���θ�� ������ ������ ���
		if (desMoveData.pos.y == (curPlayer == WHITE_PLAYER ? 0 : 7))
			Promotion(chess, desMoveData.pos, curPlayer);

		break;

	default: {
		chess->states[src.y][src.x].moveCnt++;
		chess->states[desMoveData.pos.y][desMoveData.pos.x] = chess->states[src.y][src.x]; // �⹰�� �����̰�
		chess->states[src.y][src.x] = (STATEDATA){ .pieceType = NONE, .player = EMPTY_PLAYER, .moveCnt = 0 }; // �����̱� ���� ��ġ�� NONE����
	}
	}

	// King�� ��ġ�� ã�� �κ�------------
	// ������ �÷��̾��� ���� �ƴ� ����� ��
	POS kingPos = GetKingPos(chess, chess->currentPlayer * -1);
	//-----------------------------------


	int gameState = CalculateState(chess, kingPos); // ���� � ��������, (Check, CheckMate, StaleMate)
	gotoxy(0, 31);
	if (gameState == 1)
	{
		//Check
		chess->printInfo.gameState = 1;
	}
	else if (gameState == 2)
	{
		if (CheckAround(chess, kingPos))
		{
			// CheckMate
			chess->printInfo.gameState = 2;
		}
		else
		{
			//Check
			chess->printInfo.gameState = 1;
		}
	}
	else
	{
		if (CheckAround(chess, kingPos) && !PiecesCanMove(chess, chess->currentPlayer * -1))
		{
			// StaleMate
			chess->printInfo.gameState = 3;
		}
		else
		{
			//NONE
			chess->printInfo.gameState = 0;
		}
	}


	chess->currentPlayer = chess->currentPlayer == BLACK_PLAYER ? WHITE_PLAYER : BLACK_PLAYER;

}

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
void UpdateEpState(CHESS* chess, int curColor)
{
	for (int y = 0; y < CHESS_SIZE; y++)
	{
		for (int x = 0; x < CHESS_SIZE; x++)
		{
			if( chess->states[y][x].player == curColor * -1 &&
				chess->states[y][x].pieceType == PAWN &&
				chess->states[y][x].epState == TRUE)
			{
				chess->states[y][x].epState = FALSE;
			}
		}
	}
}

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
void Promotion(CHESS* chess, POS curPos, int curColor)
{
	CursorView(0);
	PrintPromoSel(curColor);

	int ypos = GAP + 12;
	int xpos = (GAP + CHESS_SIZE * GRID_SIZE + 0) * 2 + 6;
	const int limXL = (GAP + CHESS_SIZE * GRID_SIZE + 0) * 2 + 6;
	const int limXR = (GAP + CHESS_SIZE * GRID_SIZE + 6) * 2 + 6;

	PIECETYPE piece[4] = { KNIGHT, BISHOP, ROOK, QUEEN };
	int sel = 0;
	
	while (1)
	{
		gotoxy(ypos, xpos);

		int input = _getch();
		
		// ����
		if (input == 224)
		{
			input = _getch();
			switch (input)
			{
			case RIGHT:
				if (xpos < limXR) { xpos += 4; sel++; }
				printf("  ");
				break;
			case LEFT:
				if (xpos > limXL) { xpos -= 4; sel--; }
				printf("  ");
				break;
			}
			gotoxy(ypos, xpos);
			SetColor(White, 0);
			printf("��");
		}

		// ������ �⹰�� PIECETYPE�� �ٲ۴�.
		else if (input == ENTER)
		{   
			chess->states[curPos.y][curPos.x].pieceType = piece[sel];
			break;
		}
	}
	CursorView(1);

	//Erase
	for (int y = GAP + 11; y <= GAP + 12; y++)
	{
		gotoxy(y, (GAP + CHESS_SIZE * GRID_SIZE + 2) * 2);
		printf("                ");
	}
}