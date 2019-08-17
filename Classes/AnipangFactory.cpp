#pragma once
#include"headers.h"
#include "AnipangFactory.h"
#include "AnipangGameScene.h"
#include "AnipangMap.h"
#include"AnipangManager.h"
AnipangFactory::AnipangFactory(const shared_ptr<AnipangManager>& manager) : _manager(manager)
{
	initializeEmptyColCntZero();
}
AnipangFactory::~AnipangFactory()
{
}
unique_ptr<WeaponType> AnipangFactory::setWeaponType(int type)
{
	switch (type) {
	case ARROW: return make_unique<Arrow>(); break;
	case BOOTS: return make_unique<Boots>(); break;
	case RING: return make_unique<Ring>(); break;
	case STAFF: return make_unique<Staff>(); break;
	case AXE: return make_unique<Axe>(); break;
	case BELT: return make_unique<Belt>(); break;
	case HAT : return make_unique<Hat>(); break;
	case S_ARROW: return make_unique<SpecialArrow>(); break;
	case S_BOOTS: return make_unique<SpecialBoots>(); break;
	case S_RING: return make_unique<SpecialRing>(); break;
	case S_STAFF: return make_unique<SpecialStaff>(); break;
	case S_AXE: return make_unique<SpecialAxe>(); break;
	case S_BELT: return make_unique<SpecialBelt>(); break;
	case S_HAT: return make_unique<SpecialHat>(); break;
	case BOOK:  return make_unique<Book>(); break;
	case SWORD:   return make_unique<Sword>(); break;
	}
	return make_unique<Arrow>();
}

void AnipangFactory::settingMap()
{
	_map = _manager.lock()->getAnipangMap();
	_map->straightenOut_checkMapValue();
	auto background = _map->getBackgroundNode();
	for (int col = MIN_COL_INDEX; col <= MAX_COL_INDEX; col++) {
		for (int row = MIN_ROW_INDEX; row <= MAX_ROW_INDEX; row++) {
					auto weapon = make_shared<Weapon>();
					weapon->type = setWeaponType(_map->_IntegerManagerMap[col][row]->getType());
					setWeaponSprite(weapon, weapon->type->getType());
					_map->_checkWeaponMap[col][row] = weapon;
					weapon->weaponSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
					weapon->weaponSprite->setPosition(Vec2(row*BLOCK_SIZE,col*BLOCK_SIZE));
					weapon->weaponSprite->setVisible(true);
					background->addChild(weapon->weaponSprite, WEAPON_Z_ORDER_VALUE);
		}
	}
}
int AnipangFactory::getRandomType() throw(IndexOutOfException)
{
	static random_device rd;
	static mt19937 re(rd());
	uniform_int_distribution<int> range(1, 7);

	switch (range(re)) {
	case ARROW: return ARROW; break;
	case BOOTS: return BOOTS; break;
	case RING: return RING; break;
	case STAFF: return STAFF; break;
	case AXE: return AXE; break;
	case BELT: return BELT; break;
	case HAT: return HAT; break;
	}
	return ARROW;
}

void AnipangFactory::produceNewFactor() // 새로운 퍼즐요소를 생성하는 함수
{
	initializeEmptyColCntZero();
	_map->initializeNewProduceMap();

	for (int row = MIN_ROW_INDEX; row <= MAX_ROW_INDEX; row++) {
		setEmptyColCnt(row);
		if (getEmptyColCnt(row) == 0) continue;
		else {
			setTempProducingMap(row); // 생성맵에다가 블럭 생성
			dropNewProducing(row); // 생성맵에 생성한것을 실제 게임맵에 넣는 함수
		}
	}
	_map->updateIntegerCheckMapRowAndColCnt();
}

void AnipangFactory::setEmptyColCnt(int row)
{
	int emptyColCnt = 0;
	for (int col = MIN_COL_INDEX; col <= MAX_COL_INDEX; col++) {
		if (_map->_IntegerManagerMap[col][row]->getType() == NONE)
			emptyColCnt++;
	}
	
	_emptyColCnt[row] = emptyColCnt;
}

int AnipangFactory::getEmptyColCnt(int row)
{
	return _emptyColCnt[row];
}

void AnipangFactory::initializeEmptyColCntZero()
{
	for (int initIndex = 0; initIndex < 7; initIndex++) _emptyColCnt[initIndex] = 0;
}

void AnipangFactory::setTempProducingMap(int row)
{
	_map = _manager.lock()->getAnipangMap();
	auto background = _map->getBackgroundNode();
	int colMaxIndex = getEmptyColCnt(row) - 1;
	for (int colIndex = 0; colIndex <= colMaxIndex; colIndex++) {
		auto weapon = make_shared<Weapon>();
		weapon->type = setWeaponType(getRandomType());
		setWeaponSprite(weapon, weapon->type->getType());
		_map->_newProduceMap[colIndex][row] = weapon;
		weapon->weaponSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		weapon->weaponSprite->setPosition(Vec2(row*BLOCK_SIZE, (MAX_COL_INDEX + colIndex + 1)*BLOCK_SIZE));
		weapon->weaponSprite->setVisible(true); // 일단 트루
		background->addChild(weapon->weaponSprite, WEAPON_Z_ORDER_VALUE);
	}
}

void AnipangFactory::dropNewProducing(int row)
{
	float duration = 0.24f;
	_map = _manager.lock()->getAnipangMap();
	int colCnt = getEmptyColCnt(row);
	for (int mainColIdx = colCnt - 1, newProduceColIndex = 0; mainColIdx >= 0; mainColIdx--, newProduceColIndex++) {	
		swap(_map->_checkWeaponMap[MAX_COL_INDEX - mainColIdx][row], _map->_newProduceMap[newProduceColIndex][row]);
		auto dropMove = MoveTo::create(duration*2.5 , Vec2(_map->_checkWeaponMap[MAX_COL_INDEX - mainColIdx][row]->weaponSprite->getPositionX(), _map->_checkWeaponMap[MAX_COL_INDEX - mainColIdx][row]->weaponSprite->getPositionY() - BLOCK_SIZE * (colCnt)));
		auto callback = CallFunc::create(CC_CALLBACK_0(AnipangFactory::dropNewProducingSetVisible ,this, MAX_COL_INDEX - mainColIdx, row));
		auto seq = Sequence::create(dropMove, callback, NULL);
		_map->_checkWeaponMap[MAX_COL_INDEX - mainColIdx][row]->weaponSprite->runAction(seq);
		_map->_IntegerManagerMap[MAX_COL_INDEX - mainColIdx][row]->setType(_map->_checkWeaponMap[MAX_COL_INDEX - mainColIdx][row]->type->getType());
	}
}

void AnipangFactory::dropNewProducingSetVisible(int col, int row)
{
	_manager.lock()->getScene()->GAME_STATE = CHECK_MATCHING;
	_manager.lock()->getScene()->scheduleUpdate();
}

void AnipangFactory::setWeaponSprite(shared_ptr<Weapon> weapon, int type)
{
	switch (type) {
	case ARROW: weapon->weaponSprite = Sprite::create("arrow.png"); break;
	case BOOTS:  weapon->weaponSprite = Sprite::create("boots.png"); break;
	case RING: weapon->weaponSprite = Sprite::create("ring.png"); break;
	case STAFF: weapon->weaponSprite = Sprite::create("staff.png"); break;
	case AXE: weapon->weaponSprite = Sprite::create("axe.png"); break;
	case BELT: weapon->weaponSprite = Sprite::create("belt.png"); break;
	case HAT: weapon->weaponSprite = Sprite::create("hat.png"); break;
	case S_ARROW: weapon->weaponSprite = Sprite::create("specialArrow.png"); break;
	case S_BOOTS: weapon->weaponSprite = Sprite::create("specialBoots.png"); break;
	case S_RING:  weapon->weaponSprite = Sprite::create("specialRing.png"); break;
	case S_STAFF:  weapon->weaponSprite = Sprite::create("specialStaff.png"); break;
	case S_AXE:  weapon->weaponSprite = Sprite::create("specialAxe.png"); break;
	case S_BELT: weapon->weaponSprite = Sprite::create("specialBelt.png"); break;
	case S_HAT:  weapon->weaponSprite = Sprite::create("specialHat.png"); break;
	case BOOK:  weapon->weaponSprite = Sprite::create("specialBook.png"); break;
	case SWORD:  weapon->weaponSprite = Sprite::create("sword.png"); break;
	}
}
