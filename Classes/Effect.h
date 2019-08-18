#pragma once
class AnipangManager;

class FindBingoEffect {
private:
	Sprite* findEffect[3]; // 알려주는 3개블록.
	Animation* findAnimation;
	Animate* findAnimate[3];
public:
	FindBingoEffect() {
		AnipangManager& _manager = AnipangManager::instance();
		findAnimation = Animation::create();
		findAnimation->setDelayPerUnit(0.1f);
		for (int i = 0; i < 3; i++) {
			findEffect[i] = Sprite::create("\effect\\boostV001effect000.png");
			findEffect[i]->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
			findEffect[i]->setVisible(false);
			_manager.getAnipangMap()->getBackgroundNode()->addChild(findEffect[i], EFFECT_Z_ORDER_VALUE);
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
		AnipangManager& _manager = AnipangManager::instance();
		if (!_manager.getAnipangMap()->checkDeletableBingo()) {
			_manager.getScene()->GAME_STATE = CHANGE_MAP;
			return;
		}

		auto effectInfo = _manager.getAnipangMap()->getRandomEffectBingoVector();
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
		_manager.getScene()->GAME_STATE = NOTHING;
	}
};

class Effect {
private:
	FindBingoEffect* _findEffect;
public:
	Effect() {
		_findEffect = new FindBingoEffect();
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