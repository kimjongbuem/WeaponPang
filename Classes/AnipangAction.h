#pragma once
class AnipangManager;
class AnipangMap;
class AnipangMouseEvent;
class ClickDragIndex {
private:
	int clickX, clickY, dragX, dragY;
public:
	ClickDragIndex(int cx, int cy, int dx, int dy) {
		clickX = cx; clickY = cy; dragX = dx; dragY = dy;
	}
	int getCilckX() {
		return clickX;
	}
	int getCilckY() {
		return clickY;
	}
	int getDragX() {
		return dragX;
	}
	int getDragY() {
		return dragY;
	}
	~ClickDragIndex() {

	}
};

class AnipangAction {
public:
	AnipangAction(shared_ptr<AnipangManager>& manager);
	bool swap();
	void deleteBingo();
	void reSwap();
	void actionClickMoveFinished();
	void actionDragMoveFinished();
	void reSwapClickMoveFinsided();
	void reSwapDragMoveFinsided();
	void drop();
	void dropCheck(int col, int row);
	void dropChangePosition(int col, int row, int maxDistance);
	void dropAction();
	void convertMap();
	void checkMatching();
	void bookPang();
	void swordPang();
private:
	weak_ptr<AnipangManager> _manager;
	shared_ptr<AnipangMap> _map;
private:
	ClickDragIndex* cdIndex;
	float changeTmpX1;
	float changeTmpY1;
	float changeTmpX2;
	float changeTmpY2;
	float duration = 0.28f;
	MoveTo* move1;
	MoveTo* move2;
	Sequence* seq1; Sequence* seq2;
	int dragX, dragY, clickX, clickY;
	bool isDrop = false;
	bool isRepeat = false;
private:
	void deletebyPangOfSpecial();
	void dropIsCheckedIsChangingState();
	void dropCheckingChangeNewProduceState();
	void delayDeleting(int col, int row);
	void delayNormalAndBookDeleting(int col, int row); // normal and book
	void delaySwordDeleting(int col, int row); // sword pang
};