#include"headers.h"
#include"AnipangMouseEvent.h"
#include"AnipangManager.h"
#include"AnipangMap.h"
#include "AnipangGameScene.h"
AnipangMouseEvent::AnipangMouseEvent(const shared_ptr<AnipangManager>& manager)
{
	isMove = false;
	isOnClick = false;
	_manager = manager;
	_mouseListener = EventListenerMouse::create();
	_mouseListener->onMouseDown = CC_CALLBACK_1(AnipangMouseEvent::onMouseDown, this);
	_mouseListener->onMouseUp = CC_CALLBACK_1(AnipangMouseEvent::onMouseUp, this);
	_mouseListener->onMouseMove = CC_CALLBACK_1(AnipangMouseEvent::onMouseMove, this);
	_mouseListener->onMouseScroll = CC_CALLBACK_1(AnipangMouseEvent::onMouseScroll, this);
	auto scene = _manager.lock()->getScene();
	scene->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_mouseListener, scene);
	setBackground();
}
AnipangMouseEvent::~AnipangMouseEvent() {

}
Sprite * AnipangMouseEvent::setBackground()
{
	return background = _manager.lock()->getAnipangMap()->getBackgroundNode();
}

int AnipangMouseEvent::getIndexXPosition(cocos2d::Event * event) throw (IndexOutOfException)
{
	EventMouse* e = (EventMouse*)event;
	auto sceneX = e->getCursorX();
	auto backgroundX = background->getPositionX(); 
	int indexX = (sceneX - backgroundX) / BLOCK_SIZE;
	if (indexX < MIN_COL_INDEX || indexX > MAX_COL_INDEX) throw IndexOutOfException(indexX);
	return indexX;
}

int AnipangMouseEvent::getIndexYPosition(cocos2d::Event * event) throw (IndexOutOfException)
{
	EventMouse* e = (EventMouse*)event;
	auto sceneY = e->getCursorY();
	auto backgroundY = background->getPositionY();
	int indexY = (sceneY - backgroundY) / BLOCK_SIZE;
	if (indexY < MIN_ROW_INDEX || indexY > MAX_ROW_INDEX) throw IndexOutOfException(indexY);
	return indexY;
}

void  AnipangMouseEvent::onMouseDown(cocos2d::Event * event) 
{
	try {
		clickX = getIndexXPosition(event);
		clickY = getIndexYPosition(event);
	}
	catch (IndexOutOfException e) {
		e.showIndexOfOutExceptionMessage();
	}
	auto _map = _manager.lock()->getAnipangMap();
	isOnClick = true;
	if (_map->_IntegerManagerMap[clickY][clickX]->getType() == SWORD) {
		isMove = false;
		_manager.lock()->getScene()->GAME_STATE = SWORD_PANG;
		_manager.lock()->getScene()->scheduleUpdate();
	}
	else isMove = true;
	
}

void  AnipangMouseEvent::onMouseUp(cocos2d::Event * event)
{
	isMove = false;
	isOnClick = false;
}

void  AnipangMouseEvent::onMouseMove(cocos2d::Event * event)
{
	if (isOnClick && isMove && afterMouseClickAndFinishAction) { //클릭이 되어있다면...해야할 것은 각 맵의 값들을 바꿔줘야함.. 그리고 드래그시 그방향으로 1번만 움직이게. 
		try {
			EventMouse* e = (EventMouse*)event;
			dragX = getIndexXPosition(event); dragY = getIndexYPosition(event);
			auto _map = _manager.lock()->getAnipangMap();
			if (_map->_IntegerManagerMap[dragY][dragX]->getType() == SWORD) return;
			if (clickX == dragX && clickY == dragY) return; // 같은 위치 함수 종료
			if (!checkNonMovable()) { // 대각선 제외 해야함. 일어날확률 거의 없지만...
				isMove = false;
				afterMouseClickAndFinishAction = false;
				_manager.lock()->getScene()->GAME_STATE = SWAP;
			}	
		}
		catch (IndexOutOfException e) {
			isMove = false;
			e.showIndexOfOutExceptionMessage();
		}
	}
}

void  AnipangMouseEvent::onMouseScroll(cocos2d::Event * event)
{
}

bool AnipangMouseEvent::checkNonMovable()
{
	if ((clickX - dragX >= 1 && clickY - dragY <= -1) || // 좌상
		(clickX - dragX <= -1 && clickY - dragY <= -1) ||// 우상
		(clickX - dragX >= 1 && clickY - dragY >= 1) || // 좌하
		(clickX - dragX <= -1 && clickY - dragY >= 1) ||// 우하
		(clickY - dragY  == 0 && clickX - dragX < -1) ||
		(clickY - dragY == 0 && clickX - dragX > 1) ||
		(clickY - dragY > 1 && clickX - dragX == 0) ||
		(clickY - dragY <  - 1 && clickX - dragX == 0)) return true;
	return false;
}
