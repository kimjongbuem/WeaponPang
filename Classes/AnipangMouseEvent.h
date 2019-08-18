#pragma once
class AnipangManager;
class AnipangMouseEvent {
private:
	EventListenerMouse* _mouseListener;
	cocos2d::Sprite* background;
	bool isOnClick;
	bool isMove;
	int clickY, clickX;
	int dragY, dragX;
public:
	AnipangMouseEvent(); // �Ŵ����� ���� ���������´�..
	~AnipangMouseEvent();
	Sprite* setBackground();
	int getIndexXPosition(cocos2d::Event * event);
	int getIndexYPosition(cocos2d::Event * event);
	void onMouseDown(cocos2d::Event* event);
	void onMouseUp(cocos2d::Event* event);
	void onMouseMove(cocos2d::Event* event);
	void onMouseScroll(cocos2d::Event* event);
	bool checkNonMovable(); // �����ϼ����� �κ� üũ
	boolean afterMouseClickAndFinishAction = true;
public:
	int getClickX() {
		return clickX;
	}
	int getClickY() {
		return clickY;
	}
	int getdragX() {
		return dragX;
	}
	int getdragY() {
		return dragY;
	}
};