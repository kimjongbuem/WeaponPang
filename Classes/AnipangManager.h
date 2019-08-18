#pragma once
//class AnipangMouseEvent;
#include "AnipangFactory.h"
#include "AnipangMouseEvent.h" //add
class AnipangGameScene;
class AnipangMap;
class AnipangAction;

class AnipangManager
{
public:
	static AnipangManager& instance()
	{
		if (instance_ == NULL)
		{
			instance_ = new AnipangManager();
		}
		return *instance_;
	}
private:
	AnipangManager() {}
	static AnipangManager* instance_;

private:
	shared_ptr<AnipangMap> _map; // 맵은 비교를 해야 하기 때문에 shared
	unique_ptr<AnipangFactory> _factory; // 애니팡을 만드는 공장은 하나만 있으면 되니까 unique
	AnipangGameScene* _scene;
	shared_ptr<AnipangMouseEvent> _mouseEvent;
	shared_ptr<AnipangAction> _action;
public:
	void init(AnipangGameScene *scene);
	shared_ptr<AnipangMap>& getAnipangMap();
	unique_ptr<AnipangFactory>& getAnipangFactory();
	shared_ptr<AnipangMouseEvent>& getAnipangMouseEvent();
	AnipangGameScene* getScene();
	shared_ptr<AnipangAction>& getAnipangAction();
};
