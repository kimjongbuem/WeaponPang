#pragma once
namespace ANIPANG {
	// 해당 무기들 정수 타입으로 나타냄 //
	const int BLOCK_SIZE = 100; // 100 PIXEL
	const int ARROW = 1;
	const int BOOTS = 2;
	const int RING = 3;
	const int STAFF = 4;
	const int AXE = 5;
	const int BELT = 6;
	const int HAT = 7;
	const int remainderValue = 7;
	const int specialWeaponValue = 10;
	// 스페셜 값들 // <- 터지기 네모
	const int S_ARROW = 11;
	const int S_BOOTS = 12;
	const int S_RING = 13;
	const int S_STAFF = 14;
	const int S_AXE = 15;
	const int S_BELT = 16;
	const int S_HAT = 17;
	const int specialWeaponValueEnd = 18;
	// 블럭 스왑시 해당 블럭들 전체 삭제
	const int BOOK = 99; // 특수값
	const int SWORD = 198;
	// 해당 씬과 백그라운드의 좌표값 //
	const int minXValue = 5;
	const int maxXValue = 705;
	const int minYValue = 150;
	const int maxYValue = 850;
	const int backgroundMinYValue = 0;
	const int backgroundMaxYValue = 700;
	// 배열 인덱스 //
	const int MAX_ROW = 7;
	const int MAX_COL = 7;
	const int MAX_ROW_INDEX = 6;
	const int MAX_COL_INDEX = 6;
	const int MIN_ROW_INDEX = 0;
	const int MIN_COL_INDEX = 0;
	const int NONE = -1;

	const int FIND_EFFECT_CNT2 = 0;
	const int FIND_EFFECT_CNT2_1 = 1;
	const int FIND_EFFECT_ALONE = 2;

	const int WEAPON_Z_ORDER_VALUE = 10;
	const int EFFECT_Z_ORDER_VALUE = 20;
}using namespace ANIPANG;

enum {
	SWAP,//스왑
	RESWAP, //리스왑
	DELETE_BINGO, // 빙고삭제
	DROP, // 퍼즐 밑으로..
	CHECK_MATCHING,
	SWORD_PANG,
	BOOK_PANG, // 해당 팡은 해당 5빙고가 터질때 나오는 특수블럭이 있고, 그것을 상하좌우 어떠한 1개 블럭을 스왑하면 그블럭과 일치하는 애들 모두 삭제.. 
	NEW_PRODUCE, // 재생산 후에 블록을 빈칸으로 움직이기...
	VIEW_DELETEABLE_BINGO, // 대기 4초후 (삭제 블록 보여주기 ~)
	CHANGE_MAP,
	NOTHING // 대기 4초이후 아무것도 안하는 상태
};

