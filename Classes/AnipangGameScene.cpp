#include"headers.h"

#include"AnipangGameScene.h"
#include"AnipangManager.h"
#include"AnipangAction.h"
#include"AnipangMap.h"
Scene * AnipangGameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = AnipangGameScene::create();
	scene->addChild(layer);
	return scene;
}

bool AnipangGameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Director::getInstance()->setClearColor(Color4F(0, 0, 0, 0));

	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(AnipangGameScene::menuCloseCallback, this));
	
	AnipangManager& _manager = AnipangManager::instance();
	_manager.init(this);
	_action = _manager.getAnipangAction();
	_newTime = 0.0f;
	this->scheduleUpdate();
	return true;
}
void AnipangGameScene::menuCloseCallback(cocos2d::Ref * pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void AnipangGameScene::update(float delta)
{
	AnipangManager& _manager = AnipangManager::instance();

	if (GAME_STATE != VIEW_DELETEABLE_BINGO)
		_newTime = 0.0f;

	else if(GAME_STATE != NOTHING)
		_manager.getAnipangMap()->turnoff_Effect();


	_newTime += delta;
	if (_newTime >= 4.0f) isShow = true;
	else isShow = false;


	switch (GAME_STATE)
	{
	case SWAP:
		unscheduleUpdate();
		_action->swap();
		break;
	case RESWAP:
		unscheduleUpdate();
		_action->reSwap();
		break;
	case DELETE_BINGO:
		unscheduleUpdate();
		_action->deleteBingo();
		break;
	case DROP:
		unscheduleUpdate();
		_action->drop();
		break;
	case CHECK_MATCHING:
		unscheduleUpdate();
		_action->checkMatching();
		break;
	case BOOK_PANG:
		unscheduleUpdate();
		_action->bookPang();
		break;
	case SWORD_PANG:
		unscheduleUpdate();
		_action->swordPang();
		break;
	case NEW_PRODUCE:
		unscheduleUpdate();
		_manager.getAnipangMap()->produceNewFactor();
		break;
	case VIEW_DELETEABLE_BINGO: //  유저의 입력대기상태 4초후..
		_manager.getAnipangMap()->showFindEffect(isShow);
		break;
	case CHANGE_MAP:
		_action->convertMap();
		break;
	case NOTHING:
		break;
	}

}
