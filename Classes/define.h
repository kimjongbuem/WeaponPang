#pragma once
namespace ANIPANG {
	// �ش� ����� ���� Ÿ������ ��Ÿ�� //
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
	// ����� ���� // <- ������ �׸�
	const int S_ARROW = 11;
	const int S_BOOTS = 12;
	const int S_RING = 13;
	const int S_STAFF = 14;
	const int S_AXE = 15;
	const int S_BELT = 16;
	const int S_HAT = 17;
	const int specialWeaponValueEnd = 18;
	// �� ���ҽ� �ش� ���� ��ü ����
	const int BOOK = 99; // Ư����
	const int SWORD = 198;
	// �ش� ���� ��׶����� ��ǥ�� //
	const int minXValue = 5;
	const int maxXValue = 705;
	const int minYValue = 150;
	const int maxYValue = 850;
	const int backgroundMinYValue = 0;
	const int backgroundMaxYValue = 700;
	// �迭 �ε��� //
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
	SWAP,//����
	RESWAP, //������
	DELETE_BINGO, // �������
	DROP, // ���� ������..
	CHECK_MATCHING,
	SWORD_PANG,
	BOOK_PANG, // �ش� ���� �ش� 5���� ������ ������ Ư������ �ְ�, �װ��� �����¿� ��� 1�� ���� �����ϸ� �׺��� ��ġ�ϴ� �ֵ� ��� ����.. 
	NEW_PRODUCE, // ����� �Ŀ� ����� ��ĭ���� �����̱�...
	VIEW_DELETEABLE_BINGO, // ��� 4���� (���� ��� �����ֱ� ~)
	CHANGE_MAP,
	NOTHING // ��� 4������ �ƹ��͵� ���ϴ� ����
};

