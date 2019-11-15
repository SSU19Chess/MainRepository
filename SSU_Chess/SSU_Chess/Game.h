#pragma once
#include "GameData.h"

int MainMenu(); //메인메뉴 함수, 종료 시 -1 반환

BOOL IsAvailableToMov(CHESS* ch, int y, int x, int curColor);		//해당 말이 y, x로 이동할 수 있는지 여부 반환
BOOL IsEmpty(CHESS* ch, int y, int x);								//해당 칸이 비어있으면 TRUE
BOOL OtherCanCome(CHESS* chess, const POS pos, int playerColor);	// playerColor와 다른 색의 말이 pos로 올 수 있는지 
BOOL CheckAround(CHESS* chess, const POS kingPos);					// 움직이면 죽는지 확인
BOOL IsInMap(POS pos);												// pos가 체스판 안에 있는지 확인

MOVEDATA* GetMoveData(CHESS* ch, const POS pos);					//매개변수로 전달된 체스판을 기반으로 이동가능한 위치 벡터를 반환

void Move(CHESS* chess, const POS src, const MOVEDATA desMoveData); // 기물을 움직이고 승패판정
int CalculateState(CHESS* chess, const POS kingPos);				// Check인지 CheckMate인지 StaleMate인지 판별함
																	// King이 공격을 받지 않음 == 0
																	// Check == 1
																	// 상대의 공격을 막을 수 없음 == 2

