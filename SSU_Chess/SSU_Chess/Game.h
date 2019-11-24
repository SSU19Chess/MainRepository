#pragma once
#include "GameData.h"

int InGame();
int MainMenu(); //메인메뉴 함수, 종료 시 -1 반환
void Input(CHESS* chess, POS* lastSelectPos, POS* currentPos, MOVEDATA** moveData);

CHESS* Init();

BOOL IsAvailableToMov(CHESS* ch, int y, int x, int curColor);		// 해당 말이 y, x로 이동할 수 있는지 여부 반환
BOOL IsEmpty(CHESS* ch, int y, int x);								// 해당 칸이 비어있으면 TRUE
BOOL OtherCanCome(CHESS* chess, const POS pos, int playerColor);	// playerColor와 다른 색의 말이 pos로 올 수 있는지 
BOOL CheckAround(CHESS* chess, const POS kingPos);					// 움직이면 죽는지 확인
BOOL IsInMap(POS pos);												// pos가 체스판 안에 있는지 확인
int IsInMoveData(MOVEDATA* md, const POS pos);						// pos가 MOVEDATA 목록 안에 있는지, pos와 같은 pos를 가진 MOVEDATA의 인덱스 반환, 없으면 -1

MOVEDATA* GetMoveData(CHESS* ch, const POS pos);					// 매개변수로 전달된 체스판을 기반으로 이동가능한 위치 벡터를 반환
BOOL PiecesCanMove(CHESS* chess, int curColor);						// 기물들이 움직일 수 있는 수가 있는지 확인

void Move(CHESS* chess, const POS src, const MOVEDATA desMoveData); // 기물을 움직이고 승패판정
int CalculateState(CHESS* chess, const POS kingPos);				// Check인지 CheckMate인지 StaleMate인지 판별함
																	// King이 공격을 받지 않음 == 0
																	// Check == 1
																	// 상대의 공격을 막을 수 없음 == 2

void UpdateEpState(CHESS* chess, int curColor);						// 앙파상 상태를 업데이트 한다
POS GetKingPos(CHESS* chess, int curColor);							// curColor의 King위치를 찾아서 반환한다.