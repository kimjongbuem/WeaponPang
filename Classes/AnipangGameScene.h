#pragma once
#include"headers.h"
class AnipangManager;
class AnipangAction;
class AnipangGameScene : public cocos2d::Layer{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	bool specialBook = false;
	bool firstSwapCheck = true;
	float time = 0.0f;
	int timeCnt = 0;
	int GAME_STATE;
	//bool isMove  = false;
private:
	double _newTime, _oldTime;
	void menuCloseCallback(cocos2d::Ref* pSender);
	//void scheduleCallback(float delta);
	void update(float delta);
	bool isShow = false;
	CREATE_FUNC(AnipangGameScene);
private:
	shared_ptr<AnipangAction> _action;
};