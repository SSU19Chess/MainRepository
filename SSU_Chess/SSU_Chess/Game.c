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

int MainMenu()
{
	// �ʱ� ���� -------------------------------
	//CursorView(0);

	// ���� ���� -------------------------------
	int ypos = 13;


	// ����ȭ��---------------------------------
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
	}
}

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
			if (lastSelectedPos->x == -1 || lastSelectedPos->y == -1) // �ƹ��͵� ������ �ȵ� ������ ��
			{
				if (chess->states[currentPos->y][currentPos->x].pieceType != NONE)
				{
					if (chess->states[currentPos->y][currentPos->x].player == chess->currentPlayer)
					{
						memcpy(lastSelectedPos, currentPos, sizeof(POS));
						*moveData = GetMoveData(chess, *currentPos);
					}
				}
			}
			else
			{
				int index = IsInMoveData(moveData, *currentPos);
				if (index != -1)
				{
					Move(chess, *lastSelectedPos, *(*moveData + index));
					*lastSelectedPos = (POS){ -1,-1 };
				}
			}
			r = FALSE;      //r�� ���� 0�� ���� (���ѷ��� Ż��)
			break;
		}
		}
	}
}

// ü������ �ʱ�ȭ�ϴ� �Լ�
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

	chess->kingSideCastling[0] = TRUE;
	chess->queenSideCastling[0] = TRUE;
	chess->kingSideCastling[1] = TRUE;
	chess->queenSideCastling[1] = TRUE;

	chess->currentPlayer = WHITE_PLAYER;

	return chess;
}

//�ش� ���� y, x�� �̵��� �� �ִ��� ���� ��ȯ
BOOL IsAvailableToMov(CHESS* ch, int y, int x, int curColor) 
{
	if (ch->states[y][x].player == curColor) //�ش� ĭ�� ������ �⹰�� ������ FALSE
		return FALSE;

	if(!IsInMap((POS){x, y}))
		return FALSE;
	
	return TRUE;
}

//�ش� ĭ�� ��������� TRUE
BOOL IsEmpty(CHESS* ch, int y, int x)
{
	if (!IsInMap((POS) { x, y }))
		return FALSE;

	if (ch->states[y][x].pieceType == NONE)
		return TRUE;
	else
		return FALSE;
}

// playerColor�� �ٸ� ���� ���� pos�� �� �� �ִ��� 
BOOL OtherCanCome(CHESS* chess, const POS pos, int playerColor)
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

// �����̸� �״��� Ȯ��
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

// pos�� ü���� �ȿ� �ִ��� Ȯ��
BOOL IsInMap(POS pos) 
{
	if (pos.x < 0 || pos.x >= CHESS_SIZE || pos.y < 0 || pos.y >= CHESS_SIZE)
		return FALSE;
	return TRUE;
}

// pos�� MoveData ��� �ȿ� �ִ���
int IsInMoveData(MOVEDATA* md, const POS pos)
{
	if( md == NULL)
		return -1;

	size_t size = _msize(md) / sizeof(MOVEDATA);
	for (int i = 0; i < size; i++)
	{
		if (md[i].pos.x == pos.x && md[i].pos.y == pos.y)
			return i;
	}
	return -1;
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
				POS nextPos = { pos.x, pos.y + (curColor * (i + 1)) };

				if (!IsEmpty(ch, nextPos.y, nextPos.x)) break;
				if (!IsAvailableToMov(ch, nextPos.y, nextPos.x, curColor)) break;

				ret = (MOVEDATA*)realloc(ret, sizeof(MOVEDATA) * (++cnt));

				ret[cnt - 1].isCastling = FALSE;
				ret[cnt - 1].pos.y = nextPos.y;
				ret[cnt - 1].pos.x = nextPos.x;
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

	case KNIGHT: //���� ���õ� �⹰�� KNIGHT�� ���

		for (int i = 0; i < 8; i++)
		{
			POS nextPos = { pos.x + knightMovDataX[i], pos.y + knightMovDataY[i] };
			if (IsAvailableToMov(ch, nextPos.y, nextPos.x, curColor))
			{
				ret = (MOVEDATA*)realloc(ret, sizeof(MOVEDATA) * (++cnt));
				ret[cnt - 1].pos.y = nextPos.y;
				ret[cnt - 1].pos.x = nextPos.x;
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
				ret = (MOVEDATA*)realloc(ret, sizeof(MOVEDATA) * (++cnt));
				ret[cnt - 1].pos.y = nextPos.y;
				ret[cnt - 1].pos.x = nextPos.x;

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
			POS nextPos = { pos.x + plus * bishopMovDataX[i], pos.y + plus * rookMovDataY[i] };

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

	case KING: //���� ���õ� �⹰�� KING�� ���
		
		//�Ϲ����� �̵�
		for (int i = 0; i < 8; i++)
		{
			POS nextPos = { pos.x + kingMovDataX[i], pos.y + kingMovDataY[i] };

			if (IsAvailableToMov(ch, nextPos.y, nextPos.x, curColor) && CalculateState(ch, (POS){nextPos.y, nextPos.x}))
			{
				ret = (MOVEDATA*)realloc(ret, sizeof(MOVEDATA) * (++cnt));
				ret[cnt - 1].pos.y = nextPos.y;
				ret[cnt - 1].pos.x = nextPos.x;
			}
		}

		//ĳ����
		//ŷ�� ������ ���θ� �켱 �Ǵ�
		if (ch->states[curColor == WHITE_PLAYER ? 7 : 0][curColor == WHITE_PLAYER ? 4 : 3].pieceType == KING &&
			ch->states[curColor == WHITE_PLAYER ? 7 : 0][curColor == WHITE_PLAYER ? 4 : 3].moveCnt == 0)
		{
			POS kingPos = { curColor == WHITE_PLAYER ? 7 : 0 , curColor == WHITE_PLAYER ? 4 : 3 };
			POS rookPosK = { curColor == WHITE_PLAYER ? 7 : 0, curColor == WHITE_PLAYER ? 7 : 0 };
			POS rookPosQ = { curColor == WHITE_PLAYER ? 7 : 0, curColor == WHITE_PLAYER ? 0 : 7 };

			//���� ŷ�� üũ�Ǿ��ִ��� �Ǵ�
			if (CalculateState(ch, (POS) { kingPos.x, kingPos.y }) == 1)
				break;

			//ŷ���̵� ĳ����
			//���� ������ ���� �Ǵ�
			if (ch->states[rookPosK.y][rookPosK.x].pieceType == ROOK &&
				ch->states[rookPosK.y][rookPosK.x].moveCnt == 0)
			{
				int flag = FALSE;

				for (int i = 1; i <= 2; i++) //ŷ�� �� ������ ��ĭ�� ���� �⹰�� �̵��� �������� ���� �Ǵ�
				{
					POS nextPos = { kingPos.x - (curColor * i), kingPos.y };

					if (!IsEmpty(ch, nextPos.y, nextPos.x) ||
						CalculateState(ch, (POS){nextPos.x, nextPos.y}) != 0)
						flag = TRUE;
				}
				if (!flag) 
				{
					ret = (MOVEDATA*)realloc(ret, sizeof(MOVEDATA) * (++cnt));
					ret[cnt - 1].pos.y = kingPos.y;
					ret[cnt - 1].pos.x = kingPos.x - (2 * curColor);
				}
			}
			//�����̵� ĳ����
			//���� ������ ���� �Ǵ�
			if (ch->states[rookPosQ.y][rookPosQ.x].pieceType == ROOK &&
				ch->states[rookPosQ.y][rookPosQ.x].moveCnt == 0)
			{
				int flag = FALSE;

				for (int i = 1; i <= 2; i++) //ŷ�� �� ������ ��ĭ�� ���� �⹰�� �̵��� �������� ���� �Ǵ�
				{
					POS nextPos = { kingPos.x + (curColor * i), kingPos.y };

					if (!IsEmpty(ch, nextPos.y, nextPos.x) || 
						CalculateState(ch, (POS) { nextPos.x, nextPos.y }) != 0 )
						flag = TRUE;
				}
				if (!flag)
				{
					ret = (MOVEDATA*)realloc(ret, sizeof(MOVEDATA) * (++cnt));
					ret[cnt - 1].pos.y = kingPos.y;
					ret[cnt - 1].pos.x = kingPos.x + (2 * curColor);
				}
			}
		}

		break;
	}

	return ret;
}

// �⹰�� �����̰� ��������
void Move(CHESS* chess, const POS src, const MOVEDATA desMoveData) 
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


	int gameState = CalculateState(chess, kingPos); // ���� � ��������, (Check, CheckMate, StaleMate)
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
// Check���� CheckMate���� StaleMate���� �Ǻ���
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



