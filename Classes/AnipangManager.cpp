#include"headers.h"
#include"AnipangManager.h"
#include "AnipangMouseEvent.h" //add
#include"AnipangGameScene.h"
#include"AnipangMap.h"
#include "AnipangFactory.h"
#include "AnipangAction.h"
void AnipangManager::init(AnipangGameScene * scene)
{
	_scene = scene;
	_scene->GAME_STATE = VIEW_DELETEABLE_BINGO;
	_map = make_shared<AnipangMap>(shared_from_this());
	_factory = make_unique<AnipangFactory>(shared_from_this());
	_action = make_shared<AnipangAction>(shared_from_this());
	_map->init(scene);
	_mouseEvent = make_shared<AnipangMouseEvent>(shared_from_this()); //add
}

shared_ptr<AnipangMap>& AnipangManager::getAnipangMap()
{
	return _map;
}

unique_ptr<AnipangFactory>& AnipangManager::getAnipangFactory()
{
	return _factory;
}

shared_ptr<AnipangMouseEvent>& AnipangManager::getAnipangMouseEvent()
{
	return _mouseEvent;
}

AnipangGameScene * AnipangManager::getScene()
{
	return _scene;
}

shared_ptr<AnipangAction>& AnipangManager::getAnipangAction()
{
	return _action;
}
