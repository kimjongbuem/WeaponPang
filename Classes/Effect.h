#pragma once
class AnipangManager;

class FindBingoEffect {
private:
	weak_ptr<AnipangManager> _manager;
	Sprite* findEffect[3]; // 알려주는 3개블록.
	Animation* findAnimation;
	Animate* findAnimate[3];
public:
	FindBingoEffect(weak_ptr<AnipangManager> manager) {
		_manager = manager;
		findAnimation = Animation::create();
		findAnimation->setDelayPerUnit(0.1f);
		for (int i = 0; i < 3; i++) {
			findEffect[i] = Sprite::create("\effect\\boostV001effect000.png");
			findEffect[i]->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
			findEffect[i]->setVisible(false);
			_manager.lock()->getAnipangMap()->getBackgroundNode()->addChild(findEffect[i], EFFECT_Z_ORDER_VALUE);
		}
		char fileName[64] = { 0 };
		for (int i = 0; i < 10; i++) {
			sprintf(fileName, "\effect\\boostV001effect00%d.png", i);
			findAnimation->addSpriteFrameWithFile(fileName);
		}
		for (int i = 0; i < 5; i++) {
			sprintf(fileName, "\effect\\boostV001effect01%d.png", i);
			findAnimation->addSpriteFrameWithFile(fileName);
		}
		for (int i = 0; i < 3; i++) {
			findAnimate[i] = Animate::create(findAnimation);
			findEffect[i]->runAction(RepeatForever::create(findAnimate[i]));
		}
	}
	void turnoff_Effect() {
		for(int effect = FIND_EFFECT_CNT2; effect <= FIND_EFFECT_ALONE; effect++) findEffect[effect]->setVisible(false);
	}
	void showFindBingoEffect(bool show) {
		if (!_manager.lock()->getAnipangMap()->checkDeletableBingo()) {
			_manager.lock()->getScene()->GAME_STATE = CHANGE_MAP;
			return;
		}

		auto effectInfo = _manager.lock()->getAnipangMap()->getRandomEffectBingoVector();
		for (int effect = FIND_EFFECT_CNT2; effect <= FIND_EFFECT_ALONE; effect++) {
			int colIdx = 0, rowIdx = 0;
			switch (effect)
			{
			case FIND_EFFECT_CNT2:
				colIdx = effectInfo->getCnt2Obj()->getColIndex();
				rowIdx = effectInfo->getCnt2Obj()->getRowIndex();
				break;
			case FIND_EFFECT_CNT2_1:
				colIdx = effectInfo->getCnt1Obj()->getColIndex();
				rowIdx = effectInfo->getCnt1Obj()->getRowIndex();
				break;
			case FIND_EFFECT_ALONE:
				colIdx = effectInfo->getAloneObj()->getColIndex();
				rowIdx = effectInfo->getAloneObj()->getRowIndex();
				break;
			}
			findEffect[effect]->setPosition(Vec2(rowIdx * BLOCK_SIZE, colIdx*BLOCK_SIZE));
			findEffect[effect]->setVisible(show);
		}
		_manager.lock()->getScene()->GAME_STATE = NOTHING;
	}
};

class Effect {
private:
	unique_ptr<FindBingoEffect> _findEffect;
	weak_ptr<AnipangManager> _manager;
public:
	Effect(weak_ptr<AnipangManager> manager) {
		_manager = manager;
		_findEffect = make_unique<FindBingoEffect>(manager);
	}
	void showFindBingoEffect(bool show) {
		_findEffect->showFindBingoEffect(show);

	}
	void turnoff_FindBingoEffect() {
		_findEffect->turnoff_Effect();
	}
};

//
//class DeleteBingoEffect {
//
//};