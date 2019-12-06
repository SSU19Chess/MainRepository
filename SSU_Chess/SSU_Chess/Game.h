#pragma once
#include "GameData.h"


/**
	@ 함수 이름: InGame
	@ 함수 설명: 게임화면, 게임 상태(Check, CheckMate, StaleMate)를 화면에 출력하고
				 사용자의 입력(방향키 상하좌우, Enter)을 받아 기물을 움직이는 함수를 호출
				 입력을 받고 게임에 상태에 따라 게임을 종료한다.
	@ 참조 함수들: Init, PrintBoard, PrintState, Input, PrintResult
	//
**/
int InGame();
/**
	@ 함수 이름: MainMenu
	@ 함수 설명: 게임 시작화면을 화면에 출력해주고 키보드 위, 아래 방향키를 통해 '게임 시작', '게임 종료'를 선택한다.
				 '게임 시작'을 선택하면 0을, '게임 종료'를 선택하면 -1을 반환한다.
	@ 참조 함수들: PrtMainMenu, gotoxy
**/
int MainMenu(); //메인메뉴 함수, 종료 시 -1 반환
/**
	@ 함수 이름: Input
	@ 함수 설명: getch 함수를 이용하여 '방향키 상하좌우'를 받아 체스판에서 커서를 이동하고
				 'Enter'를 입력받아서 기물을 선택하거나, 기물을 움직인다.
	@ 파라미터 이름 나열: (chess, lastSelectedPos, currentPos, moveData)
	@ 파라미터 설명
		@ chess: 체스 게임에 대한 정보를 가지고 있는 구조체
		@ lastSelectedPos: 선택한 기물의 위치
		@ currentPos: 현재 커서의 위치
		@ moveData: 선택한 기물이 이동할 수 있는 정보를 가진 구조체 포인터를 가리키는 포인터
	@ 참조 함수들: gotoxy, IsInMoveData, Move, GetMoveData
	@ exception 예외처리
	//
**/
void Input(CHESS* chess, POS* lastSelectPos, POS* currentPos, MOVEDATA** moveData);
/**
	@ 함수 이름: Init
	@ 함수 설명: CHESS 구조체를 동적할당 받아서 체스판을 초기화하고 구조체의 정보를 초기화하고 해당 구조체를 반환한다.
	@ 참조 함수들
	@ exception 예외처리
	// CHESS 구조체 동적할당에 실패하면 NULL을 반환한다.
**/
CHESS* Init();
/**
	@ 함수 이름: IsInMoveData
	@ 함수 설명: pos가 md목록 안에 있는지를 확인하여 있다면 인덱스를 반환, 없으면 -1을 반환한다.
	@ 파라미터 이름 나열 (md, pos)
	@ 파라미터 설명
		@ pos: x, y 값을 담는 구조체
		@ md: MOVEDATA 구조체 배열을 가리키는 포인터
	@ exception 예외처리
	// md가 NULL을 가리키면 -1을 반환
**/
int IsInMoveData(MOVEDATA* md, const POS pos);						// pos가 MOVEDATA 목록 안에 있는지, pos와 같은 pos를 가진 MOVEDATA의 인덱스 반환, 없으면 -1
/**
	@ 함수 이름 : IsAvailableToMov
	@ 함수 설명 : 파라미터로 전달된 말이 y, x로 이동할 수 있는지 여부 반환
	@ 파라미터 이름 나열 : (ch, y, x, curColor)
	@ 파라미터 설명 :
		@ ch : 체스 게임에 대한 정보를 가지고 있는 구조체
		@ y, x : 전달된 말의 좌표
		@ curColor : 전달된 말의 소유자
	@ 참조 함수들 : IsInMap
	@ exception 예외처리
	//
**/
BOOL IsAvailableToMov(CHESS* ch, int y, int x, int curColor);		// 해당 말이 y, x로 이동할 수 있는지 여부 반환
/**
	@ 함수 이름 : IsEmpty
	@ 함수 설명 : 해당 칸이 비어있으면 TRUE
	@ 파라미터 이름 나열 : ch, y, x
	@ 파라미터 설명 :
		@ ch : 체스 게임에 대한 정보를 가지고 있는 구조체
		@ y, x : 전달된 말의 좌표
	@ 참조 함수들 : IsInMap
	@ exception 예외처리
	//
**/
BOOL IsEmpty(CHESS* ch, int y, int x);								// 해당 칸이 비어있으면 TRUE
/**
	@ 함수 이름: IsInMap
	@ 함수 설명: pos가 체스판 범위 안에 있는지 확인하여 범위 안이면 True를 반환, 범위 밖이면 False를 반환한다.
	@ 파라미터 이름 나열 (pos)
	@ 파라미터 설명
		@ pos: x, y 값을 담는 구조체
**/
BOOL IsInMap(POS pos);												// pos가 체스판 안에 있는지 확인

/**
	@ 함수 이름: OtherCanCome
	@ 함수 설명: playerColor와 다른 색의 말이 pos로 올 수 있는지를 확인하여 결과를 반환한다.
	@ 파라미터 이름 나열 (chess, pos, playerColor)
	@ 파라미터 설명
		@ chess: 체스 게임에 대한 정보를 가지고 있는 구조체
		@ pos: playerColor와 다른 색의 말이 올 수 있는지를 확인하기 위한 위치
		@ playerColor: 검사할 플레이어의 색깔
		@ isEmpty: 해당 칸이 앞으로 비어있는지를 표시하는 변수( 폰이 올 수 있는지를 확인하기 위헤)
	@ 참조 함수들: IsInMap
	@ exception 예외처리
	// 탐색 중 체스판의 인덱스를 넘어갈 경우 for문 내에서 break를 건다.
**/
BOOL OtherCanCome(CHESS* chess, const POS pos, int playerColor, BOOL isEmpty);	// playerColor와 다른 색의 말이 pos로 올 수 있는지 
/**
	@ 함수 이름: CheckAround
	@ 함수 설명: King이 주위 8칸으로 움직일 수 없다면 True, 있으면 False를 반환한다.
	@ 파라미터 이름 나열 (chess, pos, playerColor)
	@ 파라미터 설명
		@ chess: 체스 게임에 대한 정보를 가지고 있는 구조체
		@ pos: playerColor와 다른 색의 말이 올 수 있는지를 확인하기 위한 위치
		@ playerColor: 검사할 플레이어의 색깔
	@ 참조 함수들: IsInMap
	@ exception 예외처리
	// 탐색 중 체스판의 인덱스를 넘어갈 경우 for문 내에서 break를 건다.
**/
BOOL CheckAround(CHESS* chess, const POS kingPos);					// 움직이면 죽는지 확인
/**
	@ 함수 이름: PiecesCanMove
	@ 함수 설명: curColor와 같은 색의 플레이어의 기물들이 움직일 수 있는 수가 있는지 확인하여 가능하면 True, 움직일 수 없다면 False를 반환한다.
	@ 파라미터 이름 나열 (chess, curColor)
	@ 파라미터 설명
		@ chess: 체스 게임에 대한 정보를 가지고 있는 구조체
		@ curColor: 확인할 플레이어의 색깔
	@ 참조 함수들: GetMoveData
**/
BOOL PiecesCanMove(CHESS* chess, int curColor);						// 기물들이 움직일 수 있는 수가 있는지 확인
/**
	@ 함수 이름: GetKingPos
	@ 함수 설명: curColor의 King 위치를 찾아서 반환한다.
	@ 파라미터 이름 나열 (chess, curColor)
	@ 파라미터 설명
		@ chess : 체스 게임에 대한 정보를 가지고 있는 구조체
		@ src : 전달된 플레이어의 색깔
**/
POS GetKingPos(CHESS* chess, int curColor);			// curColor의 King위치를 찾아서 반환한다.
/**
	@ 함수 이름: GetMoveData
	@ 함수 설명: 매개변수로 전달된 체스판을 기반으로 이동가능한 위치 데이터들을 반환
	@ 파라미터 이름 나열 (ch, pos)
	@ 파라미터 설명
		@ ch : 체스 게임에 대한 정보를 가지고 있는 구조체
		@ pos : x, y 값을 담는 구조체
	@ 참조 함수들 : GetKingPos, CalculateState, Move, IsAvailableToMov, IsEmpty, OtherCanCome
	@ exception 예외처리
	//
**/
MOVEDATA* GetMoveData(CHESS* ch, const POS pos);					// 매개변수로 전달된 체스판을 기반으로 이동가능한 위치 데이터들을 반환
/**
	@ 함수 이름: CalculateState
	@ 함수 설명: 현재 chess 구조체를 기반으로 게임의 상태를 계산하여 반환한다.
				 King이 상대편 기물에게 공격을 받을 위기가 아니면 0을 반환
				 Check상태이면 1을 반환
				 King이 상대편 기물에게 공격을 받을 위기인데 막을 수 없다면 2를 반환한다.
	@ 파라미터 이름 나열 (chess, kingPosa)
	@ 파라미터 설명
		@ chess: 체스 게임에 대한 정보를 가지고 있는 구조체
		@ kingPos: King의 위치
	@ 참조 함수들: IsInMap, OtherCanCome
**/
int CalculateState(CHESS* chess, const POS kingPos);
/**
	@ 함수 이름: Move
	@ 함수 설명: src에 있는 기물을 desMoveData.pos로 움직인다. 기물의 타입에 따라, King의 경우 캐슬링, Pawn의 경우 앙파상 여부를 확인하고 시행한다.
				 기물을 움직이고 게임의 상태( Check, CheckMate, StaleMate)를 확인한다.
				 위 작업이 끝난 후 상대방에게 턴을 넘긴다.
	@ 파라미터 이름 나열 (chess, src, desMoveData)
	@ 파라미터 설명
		@ chess: 체스 게임에 대한 정보를 가지고 있는 구조체
		@ src: 움직일 기물의 원래 위치
		@ desMoveData: 기물이 움직일 위치에 관한 정보를 가진 MOVEDATA 구조체
	@ 참조 함수들: UpdateEpState, Promotion, GetKingPos, CalculateState, CheckAround, PiecesCanMove
**/
void Move(CHESS* chess, const POS src, const MOVEDATA desMoveData); // 기물을 움직이고 승패판정
/**
	@ 함수 이름: UpdataEpState
	@ 함수 설명: 앙파상 상태를 업데이트 한다.
				보드 위의 앙파상이 가능한 "현재 색깔 PAWN들"을 탐색하여
				한 턴이 지나고, 앙파상이 불가능 하도록 변수를 업데이트 한다.
	@ 파라미터 이름 나열 (chess, curColor)
	@ 파라미터 설명
		@ chess : 체스 게임에 대한 정보를 가지고 있는 구조체
		@ curColor : 방금 턴을 진행한 쪽의 플레이어색
**/
void UpdateEpState(CHESS* chess, int curColor);						// 앙파상 상태를 업데이트 한다
/**
	@ 함수 이름: Promotion
	@ 함수 설명: 해당 Pawn을 프로모션한다
	@ 파라미터 이름 나열 (chess, curPos, curColor)
	@ 파라미터 설명
		@ chess : 체스 게임에 대한 정보를 가지고 있는 구조체
		@ curPos : 프로모션 할 Pawn의 위치
		@ curColor : 프로모션 할 Pawn의 소유자
	@ 참조 함수들 : CursorView
**/
void Promotion(CHESS* chess, POS curPos, int curColor);