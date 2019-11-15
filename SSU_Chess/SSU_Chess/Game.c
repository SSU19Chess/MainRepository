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

	PrtMainMenu();

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

//�Ű������� ���޵� ü������ ������� �̵������� ��ġ ���͸� ��ȯ
MOVEDATA* GetMoveData(CHESS* ch, const POS pos)
{
	MOVEDATA* ret;
	const STATEDATA curState = ch->states[pos.y][pos.x];
	const int curColor = curState.player;			 //���� �÷��̾��� color
	const int oppoColor = curColor * -1;			 //��� �÷��̾��� color

	int cnt = 0;									 //ret�� ���� ��, �̵������� ��ġ�� ����
	ret = (MOVEDATA*)malloc(sizeof(MOVEDATA) * cnt); //���̰� 0�� �� ���� �Ҵ�


	/* Ư�� PIECETYPE�� MOVEDATA */
	const int knightMovDataY[6] = { 1, 1, -1, -1, 2, -2 }; //KNIGHT�� �̵� ������
	const int knightMovDataX[6] = { 2, -2, 2, -2, 1, -1 };

	const int bishopMovDataY[4] = { 1, 1, -1, -1 };		  //BISHOP�� �̵� ������
	const int bishopMovDataX[4] = { -1, 1, -1, 1 };

	const int rookMovDataY[4] = {1, -1, 0, 0};			//ROOK�� �̵� ������
	const int rookMovDataX[4] = {0, 0, 1, -1};


	switch (curState.pieceType)
	{
	case PAWN: //pos�� ��ġ�� �⹰�� PAWN�� ���

		if (ch->states[pos.y + curColor][pos.x - 1].pieceType == oppoColor) //�밢���� �� �⹰�� �ִ��� �Ǵ�
		{
			if (IsAvailableToMov(ch, pos.y - 1, pos.x - 1, curColor))
			{
				ret = (MOVEDATA*)realloc(ret, sizeof(MOVEDATA) * (++cnt));
				ret[cnt - 1].pos.y = pos.y + curColor;
				ret[cnt - 1].pos.x = pos.x - 1;
			}
		}
		if (ch->states[pos.y + curColor][pos.x + 1].pieceType == oppoColor)
		{ 
			if (IsAvailableToMov(ch, pos.y - 1, pos.x + 1, curColor))
			{
				ret = (MOVEDATA*)realloc(ret, sizeof(MOVEDATA) * (++cnt));
				ret[cnt - 1].pos.y = pos.y + curColor;
				ret[cnt - 1].pos.x = pos.x + 1;
			}
		}

		if (curState.moveCnt == 0) //Pawn�� �ѹ��� �������� �ʾ��� ���
		{
			for (int i = 0; i <= 1; i++)
			{
				POS nextPos = { pos.y + (curColor * (i + 1)), pos.x };

				if (!IsEmpty(ch, nextPos.y, nextPos.x)) break;
				if (!IsAvailableToMov(ch, nextPos.y, nextPos.x, curColor)) break;

				ret = (MOVEDATA*)realloc(ret, sizeof(MOVEDATA) * (++cnt));

				ret[cnt - 1].isCastling = FALSE;
				ret[cnt-1].pos.y = nextPos.y;
				ret[cnt-1].pos.x = nextPos.x;
			}
		}
		else //Pawn�� �ѹ� �̻� �������� ���
		{
			if (!IsEmpty(ch, pos.y + curColor, pos.x)) break;

			ret = (MOVEDATA*)realloc(ret, sizeof(MOVEDATA) * (++cnt));
			ret[cnt - 1].pos.y = pos.y + curColor;
			ret[cnt - 1].pos.x = pos.x;
		}
		break;

	case KNIGHT: //pos�� ��ġ�� �⹰�� KNIGHT�� ���
		
		for (int i = 0; i < 6; i++)
		{
			POS nextPos = { pos.y + knightMovDataY[i], pos.x + knightMovDataX[i] };
			if (IsAvailableToMov(ch, nextPos.y, nextPos.x, curColor))
			{
				ret = (MOVEDATA*)realloc(ret, sizeof(MOVEDATA) * (++cnt));
				ret[cnt - 1].pos.y = nextPos.y;
				ret[cnt - 1].pos.x = nextPos.x;
			}
		}
		break;

	case BISHOP: //pos�� ��ġ�� �⹰�� BISHOP�� ���

		for (int i = 0; i < 4; i++)
		{
			int plus = 1;
			POS nextPos = { pos.y + plus * bishopMovDataY[i], pos.x + plus * bishopMovDataX[i] };

			while (IsAvailableToMov(ch, nextPos.y, nextPos.x, curColor))
			{
				ret = (MOVEDATA*)realloc(ret, sizeof(MOVEDATA) * (++cnt));
				ret[cnt - 1].pos.y = nextPos.y;
				ret[cnt - 1].pos.x = nextPos.x;

				nextPos.y = pos.y + plus * bishopMovDataY[i];
				nextPos.x = pos.x + plus * bishopMovDataX[i];
				plus++;
			}
		}
		break;

	case ROOK: //pos�� ��ġ�� �⹰�� ROOK�� ���
	


		for (int i = 0; i < 4; i++)
		{
			int plus = 1;
			POS nextPos = { pos.y + plus * rookMovDataY[i], pos.x + plus * bishopMovDataX[i] };

			while (IsAvailableToMov(ch, nextPos.y, nextPos.x, curColor))
			{
				ret = (MOVEDATA*)realloc(ret, sizeof(MOVEDATA) * (++cnt));
				ret[cnt - 1].pos.y = nextPos.y;
				ret[cnt - 1].pos.x = nextPos.x;

				nextPos.y = pos.y + plus * rookMovDataY[i];
				nextPos.x = pos.x + plus * rookMovDataX[i];
				plus++;
			}
		}
		break;

	case QUEEN:
		break;
	case KING:
		break;
	}

	return ret;
}

BOOL IsEmpty(CHESS* ch, int y, int x) //�ش� ĭ�� ��������� TRUE
{
	if (ch->states[y][x].pieceType == NONE)
		return TRUE;
	else
		return FALSE;
}

BOOL IsAvailableToMov(CHESS* ch, int y, int x, int curColor) //�ش� ���� y, x�� �̵��� �� �ִ��� ���� ��ȯ
{
	if (ch->states[y][x].player == curColor) //�ش� ĭ�� ������ �⹰�� ������ FALSE
		return FALSE;

	if (y < 0 || y >= 8 || x < 0 || x >= 8) //���� ���� ���̸� FALSE
		return FALSE;
	
	return TRUE;
}


void Move(CHESS* chess, const POS src, const MOVEDATA desMoveData) // �⹰�� �����̰� ��������
{
	PIECETYPE catchEnemy = NONE;

	// ������ ��ġ�� ��� ���� �ִ� ���
	if (chess->states[desMoveData.pos.y][desMoveData.pos.x].pieceType != NONE)
	{
		catchEnemy = chess->states[desMoveData.pos.y][desMoveData.pos.x].pieceType;
	}

	switch (chess->states[src.y][src.x].pieceType)
	{
	case KING: // King�� ��� ĳ���� Ȯ��
		if (desMoveData.isCastling != 0)
		{
			// Castling
			break;
		}
	default: {
		chess->states[desMoveData.pos.y][desMoveData.pos.x] = chess->states[src.y][src.x]; // �⹰�� �����̰�
		chess->states[src.y][src.x] = (STATEDATA){ .pieceType = NONE, .player = EMPTY_PLAYER, .moveCnt = 0 }; // �����̱� ���� ��ġ�� NONE����
		}
	}

	// King�� ��ġ�� ã�� �κ�------------
	POS kingPos = { 0, 0 };
	for (int y = 0; y < CHESS_SIZE; y++)
		for (int x = 0; x < CHESS_SIZE; x++)
			if (chess->states[y][x].pieceType == KING &&
				chess->states[y][x].player != chess->currentPlayer)
			{
				kingPos = (POS){ x,y };
				break;
			}
	//-----------------------------------


	int gameState = CalulateState(chess, kingPos); // ���� � ��������, (Check, CheckMate, StaleMate)
	gotoxy(0, 31);
	if (gameState == 1)
	{
		//Check
		printf("  Check  ");
	}
	else if (gameState == 2)
	{
		if (CheckAround(chess, kingPos))
		{
			// CheckMate
			printf("CheckMate");
		}
	}
	else
	{
		if (CheckAround(chess, kingPos))
		{
			// StaleMate
			printf("StaleMate");
		}
		else
		{
			printf("         ");
		}
	}


	chess->currentPlayer = chess->currentPlayer == BLACK_PLAYER ? WHITE_PLAYER : BLACK_PLAYER;

}

// King�� ������ ���� ���� == 0
// Check == 1
// ����� ������ ���� �� ���� == 2
int CalulateState(CHESS* chess, const POS kingPos) // Check���� CheckMate���� StaleMate���� �Ǻ���
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
							if (OtherCanCome(chess, _pos, color))
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
							if (OtherCanCome(chess, _pos, color))
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
								// ���� �밢��
								if ((d == 2 && s == -1) || (d == 3 && s == 1))
								{
									existEnemy = TRUE;
									int color = kingColor == BLACK_PLAYER ? WHITE_PLAYER : BLACK_PLAYER;
									if (OtherCanCome(chess, _checkPos, color))
										return 1;
								}
							}
							else
							{
								// �Ʒ��� �밢��
								if ((d == 2 && s == 1) || (d == 3 && s == -1))
								{
									existEnemy = TRUE;
									int color = kingColor == BLACK_PLAYER ? WHITE_PLAYER : BLACK_PLAYER;
									if (OtherCanCome(chess, _checkPos, color))
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
					if (OtherCanCome(chess, _checkPos, color))
						return 1;
				}
			}
		}
	}
	if (existEnemy)
		return 2;
	return 0;
}

BOOL OtherCanCome(CHESS* chess, const POS pos, int playerColor)// playerColor�� �ٸ� ���� ���� pos�� �� �� �ִ��� 
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
						if (posColor != EMPTY_PLAYER && i == 1)
						{
							if (chess->currentPlayer == BLACK_PLAYER)
							{
								// ���� �밢��
								if ((d == 2 && s == -1) || (d == 3 && s == 1))
									return TRUE;
							}
							else
							{
								// �Ʒ��� �밢��
								if ((d == 2 && s == 1) || (d == 3 && s == -1))
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

BOOL CheckAround(CHESS* chess, const POS kingPos) // �����̸� �״��� Ȯ��
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

			if (chess->states[_kingPos.y][_kingPos.x].pieceType != NONE)
			{
				// �ٸ� ���� �⹰�� �ִٸ� aroundOnlySameColor�� False�� 
				if (chess->states[_kingPos.y][_kingPos.x].player != chess->states[kingPos.y][kingPos.x].player)
					aroundOnlySameColor = FALSE;
				continue;
			}

			aroundIsFull = FALSE; // �� if���� ���� �ֺ��� �� ������ ������ �� �� ����

			if (!OtherCanCome(chess, _kingPos, chess->states[kingPos.y][kingPos.x].player))
				return FALSE;
		}
	}
	// �ֺ��� �� ������ ������� �� ���� ���̸�, ������ �� ��� ��� ����
	if (aroundOnlySameColor && aroundIsFull)
		return FALSE;
	return TRUE;

}

BOOL IsInMap(POS pos) // pos�� ü���� �ȿ� �ִ��� Ȯ��
{
	if (pos.x < 0 || pos.x >= CHESS_SIZE || pos.y < 0 || pos.y >= CHESS_SIZE)
		return FALSE;
	return TRUE;
}