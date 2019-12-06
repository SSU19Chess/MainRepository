#pragma once
#include "GameData.h"

#define GRID_SIZE 3
#define GAP 3

typedef enum _COLOR 
{ 
	Black = 0, Blue, Green, Cyan,
    Red, Magenta, Brown, Lgray, Dgray, 
	Lblue, Lgreen, Lcyan, Lred, Lmagenta, Yellow, White 
} COLOR;


/* 콘솔 관련 함수 */

/**
	@ 함수 이름: gotoxy
	@ 함수 설명: 명령 프롬프트의 특정 좌표로 이동한다.
	@ 파라미터 이름 나열 (y, x)
	@ 파라미터 설명
		@ y, x : 이동할 위치좌표
**/
void gotoxy(int y, int x);			 //gotoxy 함수

/**
	@ 함수 이름: SetColor
	@ 함수 설명: text와 background의 색상을 변경한다.
	@ 파라미터 이름 나열 (text, back)
	@ 파라미터 설명
		@ text : text 색상값
		@ back : background 색상값
**/
void SetColor(WORD text, WORD back); //text와 background의 색상을 변경한다.

/**
	@ 함수 이름: CursorView
	@ 함수 설명: 커서를 표시 여부를 업데이트 한다.
	@ 파라미터 이름 나열 (show)
	@ 파라미터 설명
		@ show : 커서표시 여부 값
**/
void CursorView(char show);          //콘솔창에 커서 숨김 여부를 설정한다.

/* 게임 출력 함수 */

/**
	@ 함수 이름: PrintBoard
	@ 함수 설명: 기본적인 체스판, 랭크와 파일을 화면에 보여주며
				 chess 구조체를 기반으로 기물을 표시하고 md 구조체를 기반으로 기물이 움직일 수 있는 위치를 표시한다.
	@ 파라미터 이름 나열 (chess, md)
	@ 파라미터 설명
		@ chess: 체스 게임에 대한 정보를 가지고 있는 구조체
		@ md: 특정 기물의 움직일 수 있는 위치에 대한 정보를 가진 구조체 배열
	@ 참조 함수들: SetColor, gotoxy, PrintGrid
**/
void PrintBoard(CHESS* chess, const MOVEDATA* md);

/**
	@ 함수 이름: PrintGrid
	@ 함수 설명: 기본적인 체스판에서 격자 1칸을 그리는 함수이다.
	@ 파라미터 이름 나열 (pos, sd, color, canMove)
	@ 파라미터 설명
		@ pos: 격자의 위치로 x, y 모두 1 ~ CHESS_SIZE 범위 안에 있다.
		@ sd: pos 위치에 있는 체스판의 정보로 플레이어의 색깔, 기물의 종류 등을 가지고 있다.
		@ color: 격자의 색깔을 가지고 있다. 흰색 or 검은색
		@ canMove: 현재 위치의 격자가 기물이 움직일 수 있는지를 알려준다. canMove가 -1이 아니면 격자 중앙이 다른 색으로 표시된다.
	@ 참조 함수들: SetColor, gotoxy
**/
void PrintGrid(POS pos, STATEDATA sd, COLOR color, int canMove);

/**
	@ 함수 이름: PrintState
	@ 함수 설명: 현재 어떤 플레이어가 수를 두어야하는 차례인지, 각 플레이어가 잡은 기물은 무엇이 있는지를 표시한다.
	@ 파라미터 이름 나열 (chess)
	@ 파라미터 설명
		@ chess: 체스 게임에 대한 정보를 가지고 있는 구조체
	@ 참조 함수들: SetColor, gotoxy, PrintRect
**/
void PrintState(CHESS* chess);

/**
	@ 함수 이름: PrintResult
	@ 함수 설명: 승리 또는 무승부를 표시하는 함수이다.
	@ 파라미터 이름 나열 (winPlayerColor)
	@ 파라미터 설명
		@ winPlayerColor: 승리한 플레이어의 색깔을 표시한다, 0은 무승부이다.
	@ 참조 함수들: SetColor, gotoxy, PrintRect
**/
void PrintResult(int winPlayerColor);

/**
	@ 함수 이름: PrintRect
	@ 함수 설명: 화면에 특수 기호를 사용하여 속이 빈 사각형을 표시하는 함수이다.
	@ 파라미터 이름 나열 (pos, width, height )
	@ 파라미터 설명
		@ pos: 사각형의 왼쪽 상단의 위치로, x 좌표는 체스판 우측 상단을 0으로 시작한다.
		@ width: 사각형 내부의 너비
		@ height: 사각형 내부의 높이
	@ 참조 함수들: SetColor, gotoxy
**/
void PrintRect(const POS pos, const int width, const int height);

/**
	@ 함수 이름: PrtMainMenu
	@ 함수 설명: 게임의 메인메뉴를 출력한다.
	@ 참조 함수들: gotoxy
**/
void PrtMainMenu();

/**
	@ 함수 이름: PrintPromoSel
	@ 함수 설명: 프로모션을 할 기물을 선택할 인터페이스를 출력한다.
	@ 파라미터 이름 나열 (curColor)
	@ 파라미터 설명
		@ curColor : 프로모션 할 Pawn의 소유자
	@ 참조 함수들 : SetColor
**/
void PrintPromoSel(int curColor);