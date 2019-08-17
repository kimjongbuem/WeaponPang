#pragma once
//class AnipangMouseEvent;
#include "AnipangFactory.h"
#include "AnipangMouseEvent.h" //add
class AnipangGameScene;
class AnipangMap;
class AnipangAction;

class AnipangManager : public enable_shared_from_this<AnipangManager> {
private:
	shared_ptr<AnipangMap> _map; // ���� �񱳸� �ؾ� �ϱ� ������ shared
	unique_ptr<AnipangFactory> _factory; // �ִ����� ����� ������ �ϳ��� ������ �Ǵϱ� unique
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