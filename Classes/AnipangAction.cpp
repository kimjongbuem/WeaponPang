#include"headers.h"
#include"AnipangAction.h"
#include"AnipangManager.h"
#include"AnipangMap.h"
#include"AnipangMouseEvent.h"
#include"AnipangGameScene.h"
#include "AnipangFactory.h"
AnipangAction::AnipangAction(shared_ptr<AnipangManager>& manager)
{
	_manager = manager;
	_map = _manager.lock()->getAnipangMap();
}

bool AnipangAction::swap()
{
	weak_ptr<AnipangMouseEvent> _mouseEvent = _manager.lock()->getAnipangMouseEvent();
	clickX = _mouseEvent.lock()->getClickX();
	clickY = _mouseEvent.lock()->getClickY();
	dragX = _mouseEvent.lock()->getdragX();
	dragY = _mouseEvent.lock()->getdragY();

	cdIndex = new ClickDragIndex(clickX, clickY, dragX, dragY);
	int weaponVal = _map->_checkWeaponMap[clickY][clickX]->type->getType();
	_map->_checkWeaponMap[clickY][clickX]->type->setType(_map->_checkWeaponMap[dragY][dragX]->type->getType());
	_map->_checkWeaponMap[dragY][dragX]->type->setType(weaponVal);


	//CHECK MAP - temp change  //
	int changeTypeVal = _map->_IntegerManagerMap[clickY][clickX]->getType();
	_map->_IntegerManagerMap[clickY][clickX]->setType(_map->_IntegerManagerMap[dragY][dragX]->getType());
	_map->_IntegerManagerMap[dragY][dragX]->setType(changeTypeVal);

	int changeColVal = _map->_IntegerManagerMap[clickY][clickX]->getColCnt();
	_map->_IntegerManagerMap[clickY][clickX]->setColCnt(_map->_IntegerManagerMap[dragY][dragX]->getColCnt());
	_map->_IntegerManagerMap[dragY][dragX]->setColCnt(changeColVal);

	int changeRowVal = _map->_IntegerManagerMap[clickY][clickX]->getRowCnt();
	_map->_IntegerManagerMap[clickY][clickX]->setRowCnt(_map->_IntegerManagerMap[dragY][dragX]->getRowCnt());
	_map->_IntegerManagerMap[dragY][dragX]->setRowCnt(changeRowVal);

	// WEAPON SPRITE MAP temp - change //
	changeTmpX1 = _map->_checkWeaponMap[clickY][clickX]->weaponSprite->getPositionX();
	changeTmpY1 = _map->_checkWeaponMap[clickY][clickX]->weaponSprite->getPositionY();
	changeTmpX2 = _map->_checkWeaponMap[dragY][dragX]->weaponSprite->getPositionX();
	changeTmpY2 = _map->_checkWeaponMap[dragY][dragX]->weaponSprite->getPositionY();
	//DelayTime* pause = DelayTime::create(duration);
	move1 = MoveTo::create(duration, Vec2(changeTmpX1, changeTmpY1));
	move2 = MoveTo::create(duration, Vec2(changeTmpX2, changeTmpY2)); // 여기다 콜백함수 추가할것임.
	auto clickCallback = CallFunc::create(CC_CALLBACK_0(AnipangAction::actionClickMoveFinished, this));
	auto dragCallback = CallFunc::create(CC_CALLBACK_0(AnipangAction::actionDragMoveFinished, this));
	seq1 = Sequence::create(move1, clickCallback, NULL); seq2 = Sequence::create(move2, dragCallback, NULL);

	_map->_checkWeaponMap[dragY][dragX].swap(_map->_checkWeaponMap[clickY][clickX]);
	_map->_checkWeaponMap[dragY][dragX]->weaponSprite->runAction(seq2);
	_map->_checkWeaponMap[clickY][clickX]->weaponSprite->runAction(seq1);

	int temp = _map->_checkWeaponMap[dragY][dragX]->type->getType();
	_map->_checkWeaponMap[dragY][dragX]->type->setType(_map->_checkWeaponMap[clickY][clickX]->type->getType());
	_map->_checkWeaponMap[clickY][clickX]->type->setType(temp);
	//교환 완료후 딜리트 상태 //
	
	return true;
}

void AnipangAction::deleteBingo()
{
	_map->setCombo(_map->getCombo() + 1);
	_map->checkAndSetIntegerBingo(); // 해당 type, cnt를 설정.
	for (int col = MIN_COL_INDEX; col <= MAX_COL_INDEX; col++) {
		for (int row = MIN_ROW_INDEX; row <= MAX_ROW_INDEX; row++) {
			if (_map->_IntegerManagerMap[col][row]->getType() == NONE) { // 해당 타입이 없다면..
				if (_map->_checkWeaponMap[col][row] == nullptr) continue;
				auto deleteDelay = DelayTime::create(duration * 0.35f);
				auto delayCallback = CallFunc::create(CC_CALLBACK_0(AnipangAction::delayNormalAndBookDeleting, this ,col, row));
				auto seq = Sequence::create(deleteDelay, delayCallback, NULL);
				_map->_checkWeaponMap[col][row]->weaponSprite->runAction(seq);
			}
			else if (_map->_IntegerManagerMap[col][row]->getType() > specialWeaponValue) {
				_map->_checkWeaponMap[col][row]->weaponSprite->retain();
				_map->_checkWeaponMap[col][row]->weaponSprite->removeFromParent();
				_map->_checkWeaponMap[col][row] = nullptr;
				_map->_checkWeaponMap[col][row] = make_shared<Weapon>();
				_manager.lock()->getAnipangFactory()->setWeaponSprite(_map->_checkWeaponMap[col][row], _map->_IntegerManagerMap[col][row]->getType());
				_map->_checkWeaponMap[col][row]->type = _manager.lock()->getAnipangFactory()->setWeaponType(_map->_IntegerManagerMap[col][row]->getType());
				_map->_checkWeaponMap[col][row]->weaponSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
				_map->_checkWeaponMap[col][row]->weaponSprite->setPosition(Vec2(row*BLOCK_SIZE, col*BLOCK_SIZE));
				_map->_checkWeaponMap[col][row]->weaponSprite->setVisible(true);
				auto background = _map->getBackgroundNode();
				background->addChild(_map->_checkWeaponMap[col][row]->weaponSprite, WEAPON_Z_ORDER_VALUE);
			}
		}
	}
	
}


void AnipangAction::reSwap()
{
	clickX = cdIndex->getCilckX(); clickY = cdIndex->getCilckY();
	dragX = cdIndex->getDragX(); dragY = cdIndex->getDragY();
	int weaponVal = _map->_checkWeaponMap[clickY][clickX]->type->getType();
	_map->_checkWeaponMap[clickY][clickX]->type->setType(_map->_checkWeaponMap[dragY][dragX]->type->getType());
	_map->_checkWeaponMap[dragY][dragX]->type->setType(weaponVal);

	int changeTypeVal = _map->_IntegerManagerMap[clickY][clickX]->getType();
	_map->_IntegerManagerMap[clickY][clickX]->setType(_map->_IntegerManagerMap[dragY][dragX]->getType());
	_map->_IntegerManagerMap[dragY][dragX]->setType(changeTypeVal);

	int changeColVal = _map->_IntegerManagerMap[clickY][clickX]->getColCnt();
	_map->_IntegerManagerMap[clickY][clickX]->setColCnt(_map->_IntegerManagerMap[dragY][dragX]->getColCnt());
	_map->_IntegerManagerMap[dragY][dragX]->setColCnt(changeColVal);

	int changeRowVal = _map->_IntegerManagerMap[clickY][clickX]->getRowCnt();
	_map->_IntegerManagerMap[clickY][clickX]->setRowCnt(_map->_IntegerManagerMap[dragY][dragX]->getRowCnt());
	_map->_IntegerManagerMap[dragY][dragX]->setRowCnt(changeRowVal);

	changeTmpX1 = _map->_checkWeaponMap[clickY][clickX]->weaponSprite->getPositionX();
	changeTmpY1 = _map->_checkWeaponMap[clickY][clickX]->weaponSprite->getPositionY();
	changeTmpX2 = _map->_checkWeaponMap[dragY][dragX]->weaponSprite->getPositionX();
	changeTmpY2 = _map->_checkWeaponMap[dragY][dragX]->weaponSprite->getPositionY();
	move1 = MoveTo::create(duration, Vec2(changeTmpX1, changeTmpY1));
	move2 = MoveTo::create(duration, Vec2(changeTmpX2, changeTmpY2)); // 여기다 콜백함수 추가할것임.
	auto clickCallback = CallFunc::create(CC_CALLBACK_0(AnipangAction::reSwapClickMoveFinsided, this));
	auto dragCallback = CallFunc::create(CC_CALLBACK_0(AnipangAction::reSwapDragMoveFinsided, this));
	seq1 = Sequence::create(move1, clickCallback, NULL); seq2 = Sequence::create(move2, dragCallback, NULL);
	_map->_checkWeaponMap[dragY][dragX].swap(_map->_checkWeaponMap[clickY][clickX]);
	_map->_checkWeaponMap[dragY][dragX]->weaponSprite->runAction(seq2);
	_map->_checkWeaponMap[clickY][clickX]->weaponSprite->runAction(seq1);

	int temp = _map->_checkWeaponMap[dragY][dragX]->type->getType();
	_map->_checkWeaponMap[dragY][dragX]->type->setType(_map->_checkWeaponMap[clickY][clickX]->type->getType());
	_map->_checkWeaponMap[clickY][clickX]->type->setType(temp);
	
	cdIndex->~ClickDragIndex(); // 소멸.
}

void AnipangAction::actionClickMoveFinished()
{
	_map->_checkWeaponMap[clickY][clickX]->weaponSprite->retain();
	_map->_checkWeaponMap[clickY][clickX]->weaponSprite->removeFromParent();
	_map->_checkWeaponMap[clickY][clickX]->weaponSprite->setPosition(Vec2(changeTmpX1, changeTmpY1));
	_map->getBackgroundNode()->addChild(_map->_checkWeaponMap[clickY][clickX]->weaponSprite);
	if (_map->_IntegerManagerMap[clickY][clickX]->getType() == BOOK) {
		_map->storeSpeicalBookIndex(dragY, dragX);
		_manager.lock()->getScene()->specialBook = true;
	}

	else if (_map->_IntegerManagerMap[dragY][dragX]->getType() == BOOK) {
		_map->storeSpeicalBookIndex(clickY, clickX);
		_manager.lock()->getScene()->specialBook = true;
	}

	if (_map->_IntegerManagerMap[dragY][dragX]->getType() == BOOK && _map->_IntegerManagerMap[clickY][clickX]->getType() == BOOK) _manager.lock()->getScene()->specialBook = false;
	_manager.lock()->getScene()->firstSwapCheck = true;
	_map->updateIntegerCheckMapRowAndColCnt(); // 갱신 //
	_manager.lock()->getScene()->GAME_STATE = CHECK_MATCHING;
	_manager.lock()->getScene()->scheduleUpdate();
}

void AnipangAction::actionDragMoveFinished()
{
	_map->_checkWeaponMap[dragY][dragX]->weaponSprite->retain();
	_map->_checkWeaponMap[dragY][dragX]->weaponSprite->removeFromParent();
	_map->_checkWeaponMap[dragY][dragX]->weaponSprite->setPosition(Vec2(changeTmpX2, changeTmpY2));
	_map->getBackgroundNode()->addChild(_map->_checkWeaponMap[dragY][dragX]->weaponSprite);

}

void AnipangAction::reSwapClickMoveFinsided()
{
	_map->_checkWeaponMap[dragY][dragX]->weaponSprite->retain();
	_map->_checkWeaponMap[dragY][dragX]->weaponSprite->removeFromParent();
	_map->_checkWeaponMap[dragY][dragX]->weaponSprite->setPosition(Vec2(changeTmpX2, changeTmpY2));
	_map->getBackgroundNode()->addChild(_map->_checkWeaponMap[dragY][dragX]->weaponSprite);
	_map->updateIntegerCheckMapRowAndColCnt();
	_manager.lock()->getScene()->GAME_STATE = VIEW_DELETEABLE_BINGO;
	_manager.lock()->getAnipangMouseEvent()->afterMouseClickAndFinishAction = true;
	_manager.lock()->getScene()->scheduleUpdate();
}

void AnipangAction::reSwapDragMoveFinsided()
{
	_map->_checkWeaponMap[dragY][dragX]->weaponSprite->retain();
	_map->_checkWeaponMap[dragY][dragX]->weaponSprite->removeFromParent();
	_map->_checkWeaponMap[dragY][dragX]->weaponSprite->setPosition(Vec2(changeTmpX2, changeTmpY2));
	_map->getBackgroundNode()->addChild(_map->_checkWeaponMap[dragY][dragX]->weaponSprite);
}

void AnipangAction::drop()
{
	isDrop = false;
	_manager.lock()->getScene()->specialBook = false;
	for (int col = MIN_COL_INDEX; col <= MAX_COL_INDEX - 1; col++) {
		for (int row = MIN_ROW_INDEX; row <= MAX_ROW_INDEX; row++) {
			if (_map->_IntegerManagerMap[col][row]->getType() == NONE) { // 해당 타입이 없다면..
				dropCheck(col, row);
			}
		}
	}
	if (!isDrop)
		dropCheckingChangeNewProduceState();
}

void AnipangAction::dropCheck(int col, int row)
{
	int maxDistance = 1;

	for (int cl = col + 1; cl <= MAX_COL_INDEX; cl++) {
		if (_map->_IntegerManagerMap[cl][row]->getType() == NONE)
			maxDistance++;
		else
			break;
	}
	dropChangePosition(col, row, maxDistance);

}

void AnipangAction::dropChangePosition(int col, int row, int maxDistance)
{
	for (int cl = col + 1; cl <= MAX_COL_INDEX; cl++) {
		if (_map->_IntegerManagerMap[cl][row]->getType() == NONE) continue;

		isDrop = true;
		_map->_IntegerManagerMap[cl - maxDistance][row]->setType(_map->_IntegerManagerMap[cl][row]->getType());
		_map->_IntegerManagerMap[cl][row]->setType(NONE);
		_map->_checkWeaponMap[cl - maxDistance][row] = move(_map->_checkWeaponMap[cl][row]);
		_map->_checkWeaponMap[cl][row] = nullptr;
		if (_map->_checkWeaponMap[cl - maxDistance][row] == nullptr) continue;
		auto dropMove = MoveTo::create(duration * 2.5, Vec2(_map->_checkWeaponMap[cl - maxDistance][row]->weaponSprite->getPositionX(), _map->_checkWeaponMap[cl - maxDistance][row]->weaponSprite->getPositionY() - BLOCK_SIZE * maxDistance));
		auto callback = CallFunc::create(CC_CALLBACK_0(AnipangAction::dropAction, this));
		auto seq = Sequence::create(dropMove, callback, NULL);
		_map->_checkWeaponMap[cl - maxDistance][row]->weaponSprite->runAction(seq);
		col++;
	}

	_map->updateIntegerCheckMapRowAndColCnt();
}

void AnipangAction::dropAction()
{
	dropIsCheckedIsChangingState();
}

void AnipangAction::convertMap()
{
	_map->convertMap();
	_manager.lock()->getScene()->GAME_STATE = VIEW_DELETEABLE_BINGO;
}

void AnipangAction::checkMatching()
{
	_map->checkMatching();
}

void AnipangAction::bookPang()
{
	_manager.lock()->getScene()->specialBook = false;
	_map->setCombo(_map->getCombo() + 1);
	int specialBookIndexX = _map->getSpeicalBookIndexX();
	int specialBookIndexY = _map->getSpeicalBookIndexY();
	int type = _map->_IntegerManagerMap[specialBookIndexY][specialBookIndexX]->getType();

	for (int col = MIN_COL_INDEX; col <= MAX_COL_INDEX; col++) {
		for (int row = MIN_ROW_INDEX; row <= MAX_ROW_INDEX; row++) {
			int curType = _map->_IntegerManagerMap[col][row]->getType();

			if (curType != NONE && (curType > specialWeaponValue && curType < specialWeaponValueEnd)) {
				_map->boomSpecialBingo(col, row);
				deletebyPangOfSpecial();
			}
			else if (_map->_IntegerManagerMap[col][row]->getType() != NONE && _map->_IntegerManagerMap[col][row]->getType() % specialWeaponValue == type % specialWeaponValue || _map->_IntegerManagerMap[col][row]->getType() == BOOK) {
				_map->_IntegerManagerMap[col][row]->setType(NONE);
				auto deleteDelay = DelayTime::create(duration * 0.35f);
				auto delayCallback = CallFunc::create(CC_CALLBACK_0(AnipangAction::delayNormalAndBookDeleting, this, col, row));
				auto seq = Sequence::create(delayCallback,NULL);
				_map->_checkWeaponMap[col][row]->weaponSprite->runAction(seq);
			}
		}
	}
}

void AnipangAction::swordPang()
{
	weak_ptr<AnipangMouseEvent> _mouseEvent = _manager.lock()->getAnipangMouseEvent();
	clickX = _mouseEvent.lock()->getClickX();
	clickY = _mouseEvent.lock()->getClickY();
	auto deleteDelay = DelayTime::create(duration * 0.35f);
	_map->setCombo(_map->getCombo() + 1);
	for (int col = MIN_COL_INDEX; col <= MAX_COL_INDEX; col++) {
		int type1 = _map->_IntegerManagerMap[col][clickX]->getType();
		int type2 = _map->_IntegerManagerMap[col][clickX]->getType();
		if (_map->_IntegerManagerMap[col][clickX]->getType() == BOOK || _map->_IntegerManagerMap[col][clickX]->getType() == SWORD && _map->_IntegerManagerMap[clickY][clickX]->getType() != SWORD) continue;
		_map->_IntegerManagerMap[col][clickX]->setType(NONE);
		_map->_IntegerManagerMap[col][clickX]->setRowCnt(NONE);
		_map->_IntegerManagerMap[col][clickX]->setColCnt(NONE);
		auto delayCallback = CallFunc::create(CC_CALLBACK_0(AnipangAction::delaySwordDeleting, this, col, clickX));
		auto seq = Sequence::create(deleteDelay, delayCallback, NULL);
		_map->_checkWeaponMap[col][clickX]->weaponSprite->runAction(seq);
	}
	for (int row = MIN_ROW_INDEX; row <= MAX_ROW_INDEX; row++) {
		int type1 = _map->_IntegerManagerMap[clickY][row]->getType();
		int type2 = _map->_IntegerManagerMap[clickY][row]->getType();
		if (_map->_IntegerManagerMap[clickY][row]->getType() == BOOK || _map->_IntegerManagerMap[clickY][row]->getType() == SWORD && _map->_IntegerManagerMap[clickY][clickX]->getType() != SWORD) continue;
		_map->_IntegerManagerMap[clickY][row]->setType(NONE);
		_map->_IntegerManagerMap[clickY][row]->setRowCnt(NONE);
		_map->_IntegerManagerMap[clickY][row]->setColCnt(NONE);
		auto delayCallback = CallFunc::create(CC_CALLBACK_0(AnipangAction::delaySwordDeleting, this, clickY, row));
		auto seq = Sequence::create(deleteDelay, delayCallback, NULL);
		_map->_checkWeaponMap[clickY][row]->weaponSprite->runAction(seq);
	}
}

void AnipangAction::deletebyPangOfSpecial()
{
	for (int col = MIN_COL_INDEX; col <= MAX_COL_INDEX; col++) {
		for (int row = MIN_ROW_INDEX; row <= MAX_ROW_INDEX; row++) {
			if (_map->_IntegerManagerMap[col][row]->getType() == NONE) {
				auto deleteDelay = DelayTime::create(duration * 0.35f);
				auto delayCallback = CallFunc::create(CC_CALLBACK_0(AnipangAction::delayNormalAndBookDeleting, this, col, row));
				auto seq = Sequence::create(delayCallback, NULL);
				_map->_checkWeaponMap[col][row]->weaponSprite->runAction(seq);
			}
		}
	}
}

void AnipangAction::dropIsCheckedIsChangingState()
{
	_manager.lock()->getScene()->GAME_STATE = CHECK_MATCHING;
	_manager.lock()->getAnipangMouseEvent()->afterMouseClickAndFinishAction = true;
	_manager.lock()->getScene()->scheduleUpdate();
}

void AnipangAction::dropCheckingChangeNewProduceState()
{
	_manager.lock()->getScene()->GAME_STATE = NEW_PRODUCE;
	_manager.lock()->getAnipangMouseEvent()->afterMouseClickAndFinishAction = true;
	_manager.lock()->getScene()->scheduleUpdate();
}

void AnipangAction::delayDeleting(int col, int row)
{
	if (_map->_checkWeaponMap[col][row] == nullptr) return; // test
	_map->_checkWeaponMap[col][row]->weaponSprite->retain();
	_map->_checkWeaponMap[col][row]->weaponSprite->removeFromParent();
	_map->_checkWeaponMap[col][row] = nullptr;

	_manager.lock()->getScene()->firstSwapCheck = false;
	_manager.lock()->getScene()->GAME_STATE = DROP;
	//_manager.lock()->getScene()->scheduleUpdate();
}

void AnipangAction::delayNormalAndBookDeleting(int col, int row)
{
	delayDeleting(col, row);
	auto chunk = _map->getBigLumpPuzzle();
	short rowIndex = (chunk->getStartRowIndex() + chunk->getEndRowIndex()) / 2;
	if (_map->getCombo() == 5 && row == rowIndex) {
		CCLOG("5_COMBO");
		short colIndex = chunk->getStartColIndex();
		_map->_IntegerManagerMap[chunk->getStartColIndex()][rowIndex]->setType(SWORD);
		auto background = _map->getBackgroundNode();
		_map->_IntegerManagerMap[chunk->getStartColIndex()][rowIndex]->setType(SWORD); // 같은 위치에 추가한다.
		_map->_checkWeaponMap[chunk->getStartColIndex()][rowIndex] = make_shared<Weapon>();
		_map->_checkWeaponMap[chunk->getStartColIndex()][rowIndex]->type = _manager.lock()->getAnipangFactory()->setWeaponType(SWORD);
		_manager.lock()->getAnipangFactory()->setWeaponSprite(_map->_checkWeaponMap[chunk->getStartColIndex()][rowIndex], SWORD);
		_map->_checkWeaponMap[chunk->getStartColIndex()][rowIndex]->weaponSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		_map->_checkWeaponMap[chunk->getStartColIndex()][rowIndex]->weaponSprite->setPosition(Vec2(rowIndex*BLOCK_SIZE, colIndex*BLOCK_SIZE));
		_map->_checkWeaponMap[chunk->getStartColIndex()][rowIndex]->weaponSprite->setVisible(true);
		background->addChild(_map->_checkWeaponMap[chunk->getStartColIndex()][rowIndex]->weaponSprite, WEAPON_Z_ORDER_VALUE);
		
		_map->setCombo(0); // 콤보 0
	}

	_manager.lock()->getScene()->scheduleUpdate();
}
void AnipangAction::delaySwordDeleting(int col, int row)
{
	delayDeleting(col, row);
	weak_ptr<AnipangMouseEvent> _mouseEvent = _manager.lock()->getAnipangMouseEvent();
	clickX = _mouseEvent.lock()->getClickX();
	clickY = _mouseEvent.lock()->getClickY();
	auto background = _map->getBackgroundNode();
	if (_map->getCombo() == 5 && clickX == row && clickY == col) {
		CCLOG("5_COMBO_sword");
		_map->_IntegerManagerMap[clickY][clickX]->setType(SWORD); // 같은 위치에 추가한다.
		_map->_checkWeaponMap[clickY][clickX] = make_shared<Weapon>();
		_map->_checkWeaponMap[clickY][clickX]->type = _manager.lock()->getAnipangFactory()->setWeaponType(SWORD);
		_manager.lock()->getAnipangFactory()->setWeaponSprite(_map->_checkWeaponMap[clickY][clickX], _map->_checkWeaponMap[clickY][clickX]->type->getType());
		_map->_checkWeaponMap[clickY][clickX]->weaponSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		_map->_checkWeaponMap[clickY][clickX]->weaponSprite->setPosition(Vec2(row*BLOCK_SIZE, col*BLOCK_SIZE));
		_map->_checkWeaponMap[clickY][clickX]->weaponSprite->setVisible(true);
		background->addChild(_map->_checkWeaponMap[clickY][clickX]->weaponSprite, WEAPON_Z_ORDER_VALUE);
		_map->setCombo(0); // 콤보 0
	}
	_manager.lock()->getScene()->scheduleUpdate();
}

