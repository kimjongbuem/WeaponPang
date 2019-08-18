#include"headers.h"
#include "AnipangMap.h"
#include "AnipangGameScene.h"
#include"AnipangManager.h"
#include"AnipangAction.h"
#include"AnipangFactory.h"
#include"ComboStore.h"
#include"Effect.h"
AnipangMap::AnipangMap()
{
	_comboStore = new ComboStore(); // 콤보 저장 하는 애 추가...
}
AnipangMap::~AnipangMap()
{
}
void AnipangMap::init(AnipangGameScene * scene)
{
	AnipangManager& _manager = AnipangManager::instance();
	_action = AnipangManager::instance().getAnipangAction();
	_action = _manager.getAnipangAction();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	background = Sprite::create("AnipangMap.png");
	background->setPosition(Vec2(minXValue,minYValue));
	background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	scene->addChild(background, 0);
	_effect = make_unique<Effect>();
	setting();
}

void AnipangMap::setting()
{
	AnipangManager& _manager = AnipangManager::instance();
	_manager.getAnipangFactory()->settingMap(); // 공장에서 만든것을 갖고온다!!!
}

Sprite * AnipangMap::getBackgroundNode()
{
	return background;
}
void AnipangMap::updateIntegerCheckMapRowAndColCnt() throw (IndexOutOfException){ // 교환한 타일의 타입의 값을 배열 업데이트(임시) 만약 해당 빙고가 안나오면 reset
	for (int col = MIN_COL_INDEX; col <= MAX_COL_INDEX; col++) {
		for (int row = MIN_ROW_INDEX; row <= MAX_ROW_INDEX; row++) {
			int colCnt = 1, rowCnt = 1;
			if (_IntegerManagerMap[col][row]->getType() == NONE) {
				_IntegerManagerMap[col][row]->setColCnt(NONE);
				_IntegerManagerMap[col][row]->setRowCnt(NONE);
				continue;
			}
			try {
				if (col - 1 < MIN_COL_INDEX)
					throw IndexOutOfException(col - 1);
				if (_IntegerManagerMap[col - 1][row]->getType() % specialWeaponValue == _IntegerManagerMap[col][row]->getType() % specialWeaponValue)
					colCnt = _IntegerManagerMap[col - 1][row]->getColCnt() + 1;
			}
			catch (IndexOutOfException e) {}
			try {
				if (row - 1 < MIN_ROW_INDEX)
					throw IndexOutOfException(row - 1);
				if (_IntegerManagerMap[col][row - 1]->getType() % specialWeaponValue == _IntegerManagerMap[col][row]->getType() % specialWeaponValue)
					rowCnt = _IntegerManagerMap[col][row - 1]->getRowCnt() + 1;
			}
			catch (IndexOutOfException e) {}
			_IntegerManagerMap[col][row]->setRowCnt(rowCnt);
			_IntegerManagerMap[col][row]->setColCnt(colCnt);
		}
	}
}

void AnipangMap::convertMap()
{
	for (int col = MIN_COL_INDEX; col <= MAX_COL_INDEX; col++) {
		for (int row = MIN_ROW_INDEX; row <= MAX_ROW_INDEX; row++) {
			_checkWeaponMap[col][row]->weaponSprite->retain();
			_checkWeaponMap[col][row]->weaponSprite->removeFromParent();
			_checkWeaponMap[col][row] = nullptr;
		}
	}
	setting();
	CCLOG("맵을 전환 합니당 !");
}

bool AnipangMap::checkDeletableBingo(){
	_randomBingoStore.clear();
	_randomBingoStore.reserve(4); // 빙고가 대충 4개정도 나올거라고 예상하여 미리 벡터 공간 4할당.
	for (int col = MIN_COL_INDEX; col <= MAX_COL_INDEX; col++) {
		for (int row = MIN_ROW_INDEX; row <= MAX_ROW_INDEX; row++) {
			if (_IntegerManagerMap[col][row]->getType() != NONE) {
				if (_IntegerManagerMap[col][row]->getColCnt() == 2) {
					checkUpperFindingBingo(col, row, col + 2, row);
					checkDownFindingBingo(col, row, col - 3, row);
					checkUpperRightFindingBingo(col, row, col + 1, row + 1, true);
					checkUpperLeftFindingBingo(col, row, col + 1, row - 1, true);
					checkDownRightFindingBingo(col, row, col - 2, row + 1, true);
					checkDownLeftFindingBingo(col, row, col - 2, row - 1, true);
				}
				else if (_IntegerManagerMap[col][row]->getRowCnt() == 2) {
					checkRightFindingBingo(col, row, col, row + 2);
					checkLeftFindingBingo(col, row, col, row - 3 );
					checkUpperRightFindingBingo(col, row, col + 1, row + 1, false);
					checkUpperLeftFindingBingo(col, row, col + 1, row - 2, false);
					checkDownRightFindingBingo(col, row, col - 1, row + 1, false);
					checkDownLeftFindingBingo(col, row, col - 1, row - 2, false);
				}
				else {
					checkShapeVFindingBingo(col, row);
					checkShapeReverseVFindingBingo(col, row);
					checkInequlityBigRightFindingBingo(col, row);
					checkInequlityBigLeftFindingBingo(col, row);
				}
			}
		}
	}
	return getRandomShowingBingoVectorIndex();
}
void AnipangMap::checkUpperFindingBingo(int cnt2Col, int cnt2Row, int alone_col, int alone_row)
{
	try {
		if (alone_col > MAX_COL_INDEX)
			throw IndexOutOfException();

		if (_IntegerManagerMap[cnt2Col][cnt2Row]->getType() == _IntegerManagerMap[alone_col][alone_row]->getType())
			_randomBingoStore.emplace_back(new FindUsingBingo(cnt2Col, cnt2Row, cnt2Col - 1, cnt2Row, alone_col, alone_row));
		
	}
	catch (IndexOutOfException e) {}
}
void AnipangMap::checkDownFindingBingo(int cnt2Col, int cnt2Row, int alone_col, int alone_row)
{
	try {
		if (alone_col < MIN_COL_INDEX)
			throw IndexOutOfException();

		if (_IntegerManagerMap[cnt2Col][cnt2Row]->getType() == _IntegerManagerMap[alone_col][alone_row]->getType())
			_randomBingoStore.emplace_back(new FindUsingBingo(cnt2Col, cnt2Row, cnt2Col - 1, cnt2Row, alone_col, alone_row));

	}
	catch (IndexOutOfException e) {}
}
void AnipangMap::checkRightFindingBingo(int cnt2Col, int cnt2Row, int alone_col, int alone_row)
{
	try {
		if (alone_row > MAX_ROW_INDEX)
			throw IndexOutOfException();

		if (_IntegerManagerMap[cnt2Col][cnt2Row]->getType() == _IntegerManagerMap[alone_col][alone_row]->getType())
			_randomBingoStore.emplace_back(new FindUsingBingo(cnt2Col, cnt2Row, cnt2Col, cnt2Row - 1, alone_col, alone_row));

	}
	catch (IndexOutOfException e) {}
}
void AnipangMap::checkLeftFindingBingo(int cnt2Col, int cnt2Row, int alone_col, int alone_row)
{
	try {
		if (alone_row < MIN_ROW_INDEX)
			throw IndexOutOfException();

		if (_IntegerManagerMap[cnt2Col][cnt2Row]->getType() == _IntegerManagerMap[alone_col][alone_row]->getType())
			_randomBingoStore.emplace_back(new FindUsingBingo(cnt2Col, cnt2Row, cnt2Col, cnt2Row - 1, alone_col, alone_row));

	}
	catch (IndexOutOfException e) {}
}
void AnipangMap::checkShapeVFindingBingo(int cnt2Col, int cnt2Row)
{
	try {
		if (cnt2Row - 1 < MIN_ROW_INDEX || cnt2Row + 1 > MAX_ROW_INDEX || cnt2Col + 1 > MAX_COL_INDEX)
			throw IndexOutOfException();

		if (_IntegerManagerMap[cnt2Col][cnt2Row]->getType() == _IntegerManagerMap[cnt2Col+1][cnt2Row - 1]->getType() && 
			_IntegerManagerMap[cnt2Col][cnt2Row]->getType() == _IntegerManagerMap[cnt2Col + 1][cnt2Row + 1]->getType())
			_randomBingoStore.emplace_back(new FindUsingBingo(cnt2Col, cnt2Row, cnt2Col + 1, cnt2Row - 1, cnt2Col + 1, cnt2Row +1));

	}
	catch (IndexOutOfException e) {}
}
void AnipangMap::checkShapeReverseVFindingBingo(int cnt2Col, int cnt2Row)
{
	try {
		if (cnt2Row - 1 < MIN_ROW_INDEX || cnt2Row + 1 > MAX_ROW_INDEX || cnt2Col - 1 < MIN_COL_INDEX)
			throw IndexOutOfException();

		if (_IntegerManagerMap[cnt2Col][cnt2Row]->getType() == _IntegerManagerMap[cnt2Col - 1][cnt2Row - 1]->getType()&&
			_IntegerManagerMap[cnt2Col][cnt2Row]->getType() == _IntegerManagerMap[cnt2Col - 1][cnt2Row + 1]->getType())
			_randomBingoStore.emplace_back(new FindUsingBingo(cnt2Col, cnt2Row, cnt2Col - 1, cnt2Row - 1, cnt2Col - 1, cnt2Row + 1));

	}
	catch (IndexOutOfException e) {}
}
void AnipangMap::checkInequlityBigRightFindingBingo(int cnt2Col, int cnt2Row)
{
	try {
		if (cnt2Col - 1 < MIN_COL_INDEX || cnt2Col + 1 > MAX_COL_INDEX || cnt2Row + 1 > MAX_ROW_INDEX)
			throw IndexOutOfException();

		if (_IntegerManagerMap[cnt2Col][cnt2Row]->getType() == _IntegerManagerMap[cnt2Col + 1][cnt2Row + 1]->getType() &&
			_IntegerManagerMap[cnt2Col][cnt2Row]->getType() == _IntegerManagerMap[cnt2Col - 1][cnt2Row + 1]->getType())
			_randomBingoStore.emplace_back(new FindUsingBingo(cnt2Col, cnt2Row, cnt2Col - 1, cnt2Row + 1, cnt2Col + 1, cnt2Row + 1));

	}
	catch (IndexOutOfException e) {}
}

void AnipangMap::checkInequlityBigLeftFindingBingo(int cnt2Col, int cnt2Row)
{
	try {
		if (cnt2Col - 1 < MIN_COL_INDEX || cnt2Col + 1 > MAX_COL_INDEX || cnt2Row - 1 < MIN_ROW_INDEX)
			throw IndexOutOfException();

		if (_IntegerManagerMap[cnt2Col][cnt2Row]->getType() == _IntegerManagerMap[cnt2Col + 1][cnt2Row - 1]->getType() &&
			_IntegerManagerMap[cnt2Col][cnt2Row]->getType() == _IntegerManagerMap[cnt2Col - 1][cnt2Row - 1]->getType())
			_randomBingoStore.emplace_back(new FindUsingBingo(cnt2Col, cnt2Row, cnt2Col - 1, cnt2Row - 1, cnt2Col + 1, cnt2Row - 1));

	}
	catch (IndexOutOfException e) {}
}

void AnipangMap::checkUpperRightFindingBingo(int cnt2Col, int cnt2Row, int alone_col, int alone_row, bool isCol)
{
	try {
		if (alone_col > MAX_COL_INDEX || alone_row > MAX_ROW_INDEX)
			throw IndexOutOfException();

		if (isCol) {
			if (_IntegerManagerMap[cnt2Col][cnt2Row]->getType() == _IntegerManagerMap[alone_col][alone_row]->getType())
				_randomBingoStore.emplace_back(new FindUsingBingo(cnt2Col, cnt2Row, cnt2Col- 1, cnt2Row, alone_col, alone_row));
		}else{
			if (_IntegerManagerMap[cnt2Col][cnt2Row]->getType() == _IntegerManagerMap[alone_col][alone_row]->getType())
				_randomBingoStore.emplace_back(new FindUsingBingo(cnt2Col, cnt2Row, cnt2Col, cnt2Row - 1, alone_col, alone_row));
		}
	}
	catch (IndexOutOfException e) {}
}

void AnipangMap::checkUpperLeftFindingBingo(int cnt2Col, int cnt2Row, int alone_col, int alone_row, bool isCol)
{
	try {
		if (alone_col > MAX_COL_INDEX || alone_row < MIN_ROW_INDEX)
			throw IndexOutOfException();

		if (isCol) {
			if (_IntegerManagerMap[cnt2Col][cnt2Row]->getType() == _IntegerManagerMap[alone_col][alone_row]->getType())
				_randomBingoStore.emplace_back(new FindUsingBingo(cnt2Col, cnt2Row, cnt2Col - 1, cnt2Row, alone_col, alone_row));
		}
		else {
			if (_IntegerManagerMap[cnt2Col][cnt2Row]->getType() == _IntegerManagerMap[alone_col][alone_row]->getType())
				_randomBingoStore.emplace_back(new FindUsingBingo(cnt2Col, cnt2Row, cnt2Col, cnt2Row - 1, alone_col, alone_row));
		}
	}
	catch (IndexOutOfException e) {}
}

void AnipangMap::checkDownRightFindingBingo(int cnt2Col, int cnt2Row, int alone_col, int alone_row, bool isCol)
{
	try {
		if (alone_col < MIN_COL_INDEX || alone_row > MAX_ROW_INDEX)
			throw IndexOutOfException();

		if (isCol) {
			if (_IntegerManagerMap[cnt2Col][cnt2Row]->getType() == _IntegerManagerMap[alone_col][alone_row]->getType())
				_randomBingoStore.emplace_back(new FindUsingBingo(cnt2Col, cnt2Row, cnt2Col - 1, cnt2Row, alone_col, alone_row));
		}
		else {
			if (_IntegerManagerMap[cnt2Col][cnt2Row]->getType() == _IntegerManagerMap[alone_col][alone_row]->getType())
				_randomBingoStore.emplace_back(new FindUsingBingo(cnt2Col, cnt2Row, cnt2Col, cnt2Row - 1, alone_col, alone_row));
		}
	}
	catch (IndexOutOfException e) {}
}

void AnipangMap::checkDownLeftFindingBingo(int cnt2Col, int cnt2Row, int alone_col, int alone_row, bool isCol)
{
	try {
		if (alone_col < MIN_COL_INDEX || alone_row < MIN_ROW_INDEX)
			throw IndexOutOfException();

		if (isCol) {
			if (_IntegerManagerMap[cnt2Col][cnt2Row]->getType() == _IntegerManagerMap[alone_col][alone_row]->getType())
				_randomBingoStore.emplace_back(new FindUsingBingo(cnt2Col, cnt2Row, cnt2Col - 1, cnt2Row, alone_col, alone_row));
		}
		else {
			if (_IntegerManagerMap[cnt2Col][cnt2Row]->getType() == _IntegerManagerMap[alone_col][alone_row]->getType())
				_randomBingoStore.emplace_back(new FindUsingBingo(cnt2Col, cnt2Row, cnt2Col, cnt2Row - 1, alone_col, alone_row));
		}
	}
	catch (IndexOutOfException e) {}
}

void AnipangMap::showFindEffect(bool show)
{
	if (show) {
		_effect->showFindBingoEffect(show);
	}
}

void AnipangMap::turnoff_Effect()
{
	_effect->turnoff_FindBingoEffect();
}

void AnipangMap::initializeNewProduceMap()
{
	for (int col = MIN_COL_INDEX; col <= MAX_COL_INDEX; col++) {
		for (int row = MIN_ROW_INDEX; row <= MAX_ROW_INDEX; row++) {
			if (_newProduceMap[col][row] == nullptr) continue;
			_newProduceMap[col][row]->weaponSprite->retain();
			_newProduceMap[col][row]->weaponSprite->removeFromParent();
			_newProduceMap[col][row] = nullptr; // 널값
		}
	}
}

void AnipangMap::checkMatching()
{
	AnipangManager& _manager = AnipangManager::instance();
	if (_manager.getScene()->specialBook) {
		_manager.getScene()->firstSwapCheck = false;
		_manager.getScene()->GAME_STATE = PANG;
		_manager.getScene()->isBookPang = true;
		_manager.getAnipangMouseEvent()->afterMouseClickAndFinishAction = true;
		_manager.getScene()->scheduleUpdate();
		return;
	}
	
	bool isItDeleted = false;
	bool isItMakingSpecial = false;
	for (int col = MIN_COL_INDEX; col <= MAX_COL_INDEX; col++) {
		for (int row = MIN_ROW_INDEX; row <= MAX_ROW_INDEX; row++) {
			if (_IntegerManagerMap[col][row]->getColCnt() >= 3 || _IntegerManagerMap[col][row]->getRowCnt() >= 3) { // 해당 타입이 없다면..
				isItDeleted = true;
			}
		}
	}
	if(isItDeleted) _manager.getScene()->GAME_STATE = DELETE_BINGO;
	else if (_manager.getScene()->firstSwapCheck) {
		_manager.getScene()->GAME_STATE = RESWAP;
		_manager.getAnipangMouseEvent()->afterMouseClickAndFinishAction = true;
		_manager.getScene()->scheduleUpdate();
		return;
	}
	else {
		if(checkGettingNoneType())  _manager.getScene()->GAME_STATE = NEW_PRODUCE;
		else _manager.getScene()->GAME_STATE = VIEW_DELETEABLE_BINGO;

		_manager.getAnipangMouseEvent()->afterMouseClickAndFinishAction = true;
		_manager.getScene()->scheduleUpdate();
		return;
	}
	_manager.getScene()->firstSwapCheck = false;
	_manager.getAnipangMouseEvent()->afterMouseClickAndFinishAction = true;
	_manager.getScene()->scheduleUpdate();
}

void AnipangMap::storeSpeicalBookIndex(int bookIndexY, int bookIndexX)
{
	specialBookX =  bookIndexX;
	specialBookY = bookIndexY;
}

int AnipangMap::getSpeicalBookIndexX()
{
	return specialBookX;
}

int AnipangMap::getSpeicalBookIndexY()
{
	return specialBookY;
}

int AnipangMap::getCombo()
{
	return _comboStore->getCombo();
}

void AnipangMap::setCombo(int combo)
{
	_comboStore->setCombo(combo);
}

void AnipangMap::makeLump()
{
	_lumps.clear();
	_lumps.reserve(5);
	for (int col = MIN_COL_INDEX; col <= MAX_COL_INDEX; col++) {
		for (int row = MIN_ROW_INDEX; row <= MAX_ROW_INDEX; row++) {
			if (row - 1 >= MIN_ROW_INDEX && _IntegerManagerMap[col][row - 1]->getType() == NONE) continue;
			else if (_IntegerManagerMap[col][row]->getType() == NONE) {
				setLump(col, row);
			}
		}
	}
}

void AnipangMap::setLump(int col, int row)
{
	int cnt = 0;
	int endRow = 0;
	for (int c = col; c <= MAX_COL_INDEX; c++) {
		if (_IntegerManagerMap[c][row]->getType() != NONE) break;// 해당 col인덱스부분이 비어있지 않다면 break
			
		for (int r = row; r <= MAX_ROW_INDEX; r++) {
			if (_IntegerManagerMap[c][r]->getType() == NONE) {
				if(col == c) endRow = r; // 맨밑에 col Index해당경우
				++cnt;
			}
			else break; // row 부분이 비어있지않다면 break
		}
	}
	_lumps.emplace_back(new LumpPuzzle(col, row, cnt, endRow));
}

LumpPuzzle * AnipangMap::getBigLumpPuzzle()
{
	makeLump();
	int bigLumpIndex = 0;
	for (int idx = 1; idx < _lumps.size(); idx++) {
		if (_lumps[idx]->getLumpCnt() > _lumps[bigLumpIndex]->getLumpCnt()) bigLumpIndex = idx;
	}
	return _lumps[bigLumpIndex];
}

bool AnipangMap::checkGettingNoneType()
{
	for (int col = MAX_COL_INDEX; col >= 0; col--) {
		for (int row = MAX_ROW_INDEX; row >= 0; row--) {
			if (_IntegerManagerMap[col][row]->getType() == NONE)
				return true;
		}
	}
	return false;
}

bool AnipangMap::getRandomShowingBingoVectorIndex()
{	if(_randomBingoStore.size() == 0)
		return false;

	static random_device rd2;
	static mt19937 re2(rd2());
	uniform_int_distribution<int> index(0 , _randomBingoStore.size() - 1);
	
	storeIndex = index(re2);
	
	return true;
}

void AnipangMap::setIntegerTypeAndCheckSpecialType(int col, int row, int bingo, bool isCol)
{
	if (isCol) { // 해당 col
		deleteAroundSpecialBlockIsColBingoTrue(col, row);
	}
	deleteAroundSpecialBlockIsRowBingoTrue(col, row);
}

bool AnipangMap::deleteAroundSpecialBlockIsCol4BingoTrue(int col, int row)
{
	int type = _IntegerManagerMap[col][row]->getType();
	try {
			for (int cnt = 1; cnt <= 2; cnt++) {
				if (_IntegerManagerMap[col - cnt][row]->getRowCnt() == 3 && _IntegerManagerMap[col - cnt][row]->getType() == type) {
					for (int sub = 0; sub < 3; sub++) {
						_IntegerManagerMap[col - cnt][row - sub]->setColCnt(NONE);
						_IntegerManagerMap[col - cnt][row - sub]->setRowCnt(NONE);
						_IntegerManagerMap[col - cnt][row - sub]->setType(NONE);
						if (_checkWeaponMap[col - cnt][row - sub] == nullptr) continue;
						_checkWeaponMap[col - cnt][row - sub]->type->setType(NONE);
					}
					deleteAround4BingoColSpecialValue(col, row, cnt, type);
					return true;
				}
				if (row + 2 > MAX_ROW_INDEX)
					throw IndexOutOfException();

				if (_IntegerManagerMap[col - cnt][row + 2]->getRowCnt() == 3 && _IntegerManagerMap[col - cnt][row]->getType() == _IntegerManagerMap[col][row]->getType()) {
					for (int sub = 0; sub < 3; sub++) {
						_IntegerManagerMap[col - cnt][row + 2 - sub]->setColCnt(NONE);
						_IntegerManagerMap[col - cnt][row + 2 - sub]->setRowCnt(NONE);
						_IntegerManagerMap[col - cnt][row + 2 - sub]->setType(NONE);
						if (_checkWeaponMap[col - cnt][row + 2 - sub] == nullptr) continue;
						_checkWeaponMap[col - cnt][row + 2 - sub]->type->setType(NONE);
					}
					deleteAround4BingoColSpecialValue(col, row, cnt, type);
					return true;
			}
		}
	}
	catch (IndexOutOfException e) {}
	return false;
}

bool AnipangMap::deleteAroundSpecialBlockIsCol3BingoTrue(int col, int row)
{
	int type = _IntegerManagerMap[col][row]->getType();
	try {
		for (int cnt = 0; cnt <= 2; cnt++) {
			for (int add = 0; add < 3; add++) {
				if (row + add > MAX_ROW_INDEX)
					throw IndexOutOfException();
				if (_IntegerManagerMap[col - cnt][row + add]->getRowCnt() == 3 && _IntegerManagerMap[col - cnt][row + add]->getType() == type) {
					for (int i = 0; i < 3; i++) {
						if (_IntegerManagerMap[col - cnt][row + add - i]->getType() > specialWeaponValue) boomSpecialBingo(col - cnt, row + add - i);
						_IntegerManagerMap[col - cnt][row + add - i]->setColCnt(NONE);
						_IntegerManagerMap[col - cnt][row + add - i]->setRowCnt(NONE);
						_IntegerManagerMap[col - cnt][row + add - i]->setType(NONE);
						if (_checkWeaponMap[col - cnt][row + add - i] == nullptr) continue;
						_checkWeaponMap[col - cnt][row + add - i]->type->setType(NONE);
					}
					for (int cnt = 0; cnt < 3; cnt++) {
						if (_IntegerManagerMap[col - cnt][row]->getType() > specialWeaponValue) boomSpecialBingo(col - cnt, row);
						_IntegerManagerMap[col - cnt][row]->setRowCnt(NONE);
						_IntegerManagerMap[col - cnt][row]->setColCnt(NONE);
						_IntegerManagerMap[col - cnt][row]->setType(NONE);
						if (_checkWeaponMap[col - cnt][row] == nullptr) continue; //test용
						_checkWeaponMap[col - cnt][row]->type->setType(NONE);
					}
					_IntegerManagerMap[col - cnt][row]->setType(type + specialWeaponValue);
					return true;
				}
			}
		}
	}
	catch (IndexOutOfException e) {}
	return false;
}
bool AnipangMap::deleteAroundSpecialBlockIsRow4BingoTrue(int col, int row)
{
	int type = _IntegerManagerMap[col][row]->getType();
	try {
		for (int cnt = 1; cnt <= 2; cnt++) {
			if (_IntegerManagerMap[col][row - cnt]->getColCnt() == 3 && _IntegerManagerMap[col][row -cnt]->getType() == type) {
				for (int sub = 0; sub < 3; sub++) {
					if (_IntegerManagerMap[col - sub][row - cnt]->getType() > specialWeaponValue) boomSpecialBingo(col - sub, row - cnt);
					_IntegerManagerMap[col - sub][row - cnt]->setColCnt(NONE);
					_IntegerManagerMap[col - sub][row - cnt]->setRowCnt(NONE);
					_IntegerManagerMap[col - sub][row - cnt]->setType(NONE);
					if (_checkWeaponMap[col - sub][row - cnt] == nullptr) continue;
					_checkWeaponMap[col - sub][row - cnt]->type->setType(NONE);
				}
				deleteAround4BingoRowSpecialValue(col, row, cnt, type);
				return true;
			}
			if (col + 2 > MAX_COL_INDEX)
				throw IndexOutOfException();

			if (_IntegerManagerMap[col + 2][row]->getColCnt() == 3 && _IntegerManagerMap[col + 2][row]->getType() == type) {
				for (int sub = 0; sub < 3; sub++) {
					if (_IntegerManagerMap[col + 2 - sub][row - cnt]->getType() > specialWeaponValue) boomSpecialBingo(col + 2 - sub, row - cnt);
					_IntegerManagerMap[col + 2 - sub][row - cnt]->setColCnt(NONE);
					_IntegerManagerMap[col + 2 - sub][row - cnt]->setRowCnt(NONE);
					_IntegerManagerMap[col + 2 - sub][row - cnt]->setType(NONE);
					if (_checkWeaponMap[col + 2 - sub][row - cnt] == nullptr) continue;
					_checkWeaponMap[col + 2 - sub][row - cnt]->type->setType(NONE);
				}
				deleteAround4BingoRowSpecialValue(col, row, cnt, type);
				return true;
			}
		}
	}
	catch (IndexOutOfException e) {}
	return false;
}
bool AnipangMap::deleteAroundSpecialBlockIsRow3BingoTrue(int col, int row)
{
	int type = _IntegerManagerMap[col][row]->getType();
	try {
		for (int cnt = 0; cnt <= 2; cnt++) {
			for (int add = 0; add < 3; add++) {
				if (col + add > MAX_COL_INDEX)
					throw IndexOutOfException();
				if (_IntegerManagerMap[col + add][row - cnt]->getColCnt() == 3 && _IntegerManagerMap[col + add][row - cnt]->getType() == type) {
					for (int i = 0; i < 3; i++) {
						_IntegerManagerMap[col + add - i][row - cnt]->setColCnt(NONE);
						_IntegerManagerMap[col + add - i][row - cnt]->setRowCnt(NONE);
						_IntegerManagerMap[col + add - i][row - cnt]->setType(NONE);
						if (_checkWeaponMap[col + add - i][row - cnt] == nullptr) continue;
						_checkWeaponMap[col + add - i][row - cnt]->type->setType(NONE);
					}
					for (int cnt = 0; cnt < 3; cnt++) {
						_IntegerManagerMap[col][row - cnt]->setRowCnt(NONE);
						_IntegerManagerMap[col][row - cnt]->setColCnt(NONE);
						_IntegerManagerMap[col][row - cnt]->setType(NONE);
						if (_checkWeaponMap[col][row - cnt] == nullptr) continue; //test용
						_checkWeaponMap[col][row - cnt]->type->setType(NONE);
					}
					_IntegerManagerMap[col][row - cnt]->setType(type + specialWeaponValue);
					return true;
				}
			}
		}
	}
	catch (IndexOutOfException e) {}
	return false;
}
void AnipangMap::deleteAround4BingoRowSpecialValue(int col, int row, int sub, int type)
{
	for (int cnt = 0; cnt < 4; cnt++) {
		_IntegerManagerMap[col][row - cnt]->setColCnt(NONE);
		_IntegerManagerMap[col][row - cnt]->setRowCnt(NONE);
		_IntegerManagerMap[col][row - cnt]->setType(NONE);
		if (_checkWeaponMap[col][row - cnt] == nullptr) continue; //test용
		_checkWeaponMap[col][row - cnt]->type->setType(NONE);
	}
	_IntegerManagerMap[col][row - sub]->setType(type + specialWeaponValue);
	_checkWeaponMap[col][row - sub]->type->setType(NONE);
}
void AnipangMap::deleteAround4BingoColSpecialValue(int col, int row, int sub, int type)
{
	for (int cnt = 0; cnt < 4; cnt++) {
		_IntegerManagerMap[col - cnt][row]->setRowCnt(NONE);
		_IntegerManagerMap[col - cnt][row]->setColCnt(NONE);
		_IntegerManagerMap[col - cnt][row]->setType(NONE);
		if (_checkWeaponMap[col - cnt][row] == nullptr) continue; //test용
		_checkWeaponMap[col - cnt][row]->type->setType(NONE);
	}

	_IntegerManagerMap[col - sub][row]->setType(type + specialWeaponValue);
	_checkWeaponMap[col - sub][row]->type->setType(type + specialWeaponValue);
}
void AnipangMap::boomSpecialBingo(int col, int row)
{
	for (int c = col - 1; c <= col + 1; c++) {
		for (int r = row - 1; r <= row + 1; r++) {
			if (c > MAX_COL_INDEX || c < MIN_COL_INDEX || r > MAX_ROW_INDEX || r < MIN_ROW_INDEX) continue;
			if (_IntegerManagerMap[c][r]->getType() == BOOK) continue;
			else if (_IntegerManagerMap[c][r]->getType() > specialWeaponValue) {
				_IntegerManagerMap[c][r]->setRowCnt(NONE);
				_IntegerManagerMap[c][r]->setColCnt(NONE);
				_IntegerManagerMap[c][r]->setType(NONE);
				boomSpecialBingo(c, r); // 재귀
			}
			else {
				_IntegerManagerMap[c][r]->setRowCnt(NONE);
				_IntegerManagerMap[c][r]->setColCnt(NONE);
				_IntegerManagerMap[c][r]->setType(NONE);
			}
		}
	}
}
void AnipangMap::deleteAroundSpecialBlockIsColBingoTrue(int col, int row)
{
	int type = _IntegerManagerMap[col][row]->getType();
	int bingo = _IntegerManagerMap[col][row]->getColCnt();
	try {
		if (bingo == 5) { // 여기 수정 해야함 COL, 안함
			if (deleteAroundSpecialBlockIsCol5BingoTrue(col, row)) {}
			else {
				for (int cnt = 0; cnt < bingo; cnt++) {
					// 해당 라인이 스왑 되었을시 이벤트가 발생하도록 하게해야함.
					_IntegerManagerMap[col - cnt][row]->setRowCnt(NONE);
					_IntegerManagerMap[col - cnt][row]->setColCnt(NONE);
					_IntegerManagerMap[col - cnt][row]->setType(NONE);
					if (_checkWeaponMap[col - cnt][row] == nullptr) continue; //test용
					_checkWeaponMap[col - cnt][row]->type->setType(NONE);
				}
				_IntegerManagerMap[col - 2][row]->setType(BOOK);
				_checkWeaponMap[col - 2][row]->type->setType(BOOK);
			}
		}
		else if (bingo == 4) {
			if (deleteAroundSpecialBlockIsCol4BingoTrue(col, row)) {}
			else {
				for (int cnt = 0; cnt < 4; cnt++) {
					if (_IntegerManagerMap[col - cnt][row]->getType() > specialWeaponValue) boomSpecialBingo(col -cnt, row);
					_IntegerManagerMap[col - cnt][row]->setRowCnt(NONE);
					_IntegerManagerMap[col - cnt][row]->setColCnt(NONE);
					_IntegerManagerMap[col - cnt][row]->setType(NONE);
					if (_checkWeaponMap[col - cnt][row] == nullptr) continue; //test용
					_checkWeaponMap[col - cnt][row]->type->setType(NONE);
				}
				_IntegerManagerMap[col - 2][row]->setType(type + specialWeaponValue);
			}
		}
		else if (bingo == 3) {
			if (deleteAroundSpecialBlockIsCol3BingoTrue(col, row)){}
			else{
				for (int cnt = 0; cnt < bingo; cnt++) {
					if (_IntegerManagerMap[col - cnt][row]->getType() > specialWeaponValue) boomSpecialBingo(col - cnt, row);		
					_IntegerManagerMap[col - cnt][row]->setRowCnt(NONE);
					_IntegerManagerMap[col - cnt][row]->setColCnt(NONE);
					_IntegerManagerMap[col - cnt][row]->setType(NONE);
					if (_checkWeaponMap[col - cnt][row] == nullptr) continue; //test용
					_checkWeaponMap[col - cnt][row]->type->setType(NONE);
				}
			}
		}

	}catch(IndexOutOfException e){}
}

void AnipangMap::deleteAroundSpecialBlockIsRowBingoTrue(int col, int row)
{
	int type = _IntegerManagerMap[col][row]->getType();
	int bingo = _IntegerManagerMap[col][row]->getRowCnt();
	try {
		if (bingo == 5) { // 여기 수정 해야함 COL, 안함
			if (deleteAroundSpecialBlockIsRow5BingoTrue(col, row)) {}
			else {
				for (int cnt = 0; cnt < bingo; cnt++) {
					// 해당 라인이 스왑 되었을시 이벤트가 발생하도록 하게해야함.
					_IntegerManagerMap[col][row - cnt]->setRowCnt(NONE);
					_IntegerManagerMap[col][row - cnt]->setColCnt(NONE);
					_IntegerManagerMap[col][row - cnt]->setType(NONE);
					if (_checkWeaponMap[col][row - cnt] == nullptr) continue; //test용
					_checkWeaponMap[col][row - cnt]->type->setType(NONE);
				}
				_IntegerManagerMap[col][row - 2]->setType(BOOK);
				_checkWeaponMap[col][row - 2]->type->setType(BOOK);
			}
		}
		else if (bingo == 4) {
			if (deleteAroundSpecialBlockIsRow4BingoTrue(col, row)) {}
			else {
				for (int cnt = 0; cnt < bingo; cnt++) {
					if (_IntegerManagerMap[col][row - cnt]->getType() > specialWeaponValue) boomSpecialBingo(col, row - cnt);
					_IntegerManagerMap[col][row - cnt]->setRowCnt(NONE);
					_IntegerManagerMap[col][row - cnt]->setColCnt(NONE);
					_IntegerManagerMap[col][row - cnt]->setType(NONE);
					if (_checkWeaponMap[col][row - cnt] == nullptr) continue; //test용
					_checkWeaponMap[col][row - cnt]->type->setType(NONE);
				}
				_IntegerManagerMap[col][row- 2]->setType(type + specialWeaponValue);
			}
		}
		else if (bingo == 3) {
			if (deleteAroundSpecialBlockIsRow3BingoTrue(col, row)) {}
			else {
				for (int cnt = 0; cnt < bingo; cnt++) {
					if (_IntegerManagerMap[col][row - cnt]->getType() > specialWeaponValue) boomSpecialBingo(col, row - cnt);
					_IntegerManagerMap[col][row - cnt]->setColCnt(NONE);
					_IntegerManagerMap[col][row - cnt]->setRowCnt(NONE);
					_IntegerManagerMap[col][row - cnt]->setType(NONE);
					if (_checkWeaponMap[col][row - cnt] == nullptr) continue; //test용
					_checkWeaponMap[col][row - cnt]->type->setType(NONE);
				}
			}
		}

	}
	catch (IndexOutOfException e) {}
}

bool AnipangMap::deleteAroundSpecialBlockIsCol5BingoTrue(int col, int row)
{
	int type = _IntegerManagerMap[col][row]->getType();
	try {
		if (_IntegerManagerMap[col - 2][row]->getRowCnt() == 3 && _IntegerManagerMap[col - 2][row]->getType() == type) {
			for (int cnt = 0; cnt < 5; cnt++) {
				_IntegerManagerMap[col - cnt][row]->setRowCnt(NONE);
				_IntegerManagerMap[col - cnt][row]->setColCnt(NONE);
				_IntegerManagerMap[col - cnt][row]->setType(NONE);
			}
			for (int cnt = 0; cnt < 3; cnt++) {
				_IntegerManagerMap[col - 2][row - cnt]->setRowCnt(NONE);
				_IntegerManagerMap[col - 2][row - cnt]->setColCnt(NONE);
				_IntegerManagerMap[col - 2][row - cnt]->setType(NONE);
			}
			return true;
		}

		if (row + 2 > MAX_ROW_INDEX)
			throw IndexOutOfException();

		if (_IntegerManagerMap[col - 2][row + 2]->getRowCnt() == 3 && _IntegerManagerMap[col - 2][row + 2]->getType() == type) {
			for (int cnt = 0; cnt < 5; cnt++) {
				_IntegerManagerMap[col - cnt][row]->setRowCnt(NONE);
				_IntegerManagerMap[col - cnt][row]->setColCnt(NONE);
				_IntegerManagerMap[col - cnt][row]->setType(NONE);
			}
			for (int cnt = 0; cnt < 3; cnt++) {
				_IntegerManagerMap[col - 2][row + cnt]->setRowCnt(NONE);
				_IntegerManagerMap[col - 2][row + cnt]->setColCnt(NONE);
				_IntegerManagerMap[col - 2][row + cnt]->setType(NONE);
			}
			return true;
		}
	}
	catch (IndexOutOfException e) {}
	return false;
}

bool AnipangMap::deleteAroundSpecialBlockIsRow5BingoTrue(int col, int row)
{
	int type = _IntegerManagerMap[col][row]->getType();
	try {
		if (_IntegerManagerMap[col][row - 2]->getColCnt() == 3 && _IntegerManagerMap[col][row - 2]->getType() == type) {
			for (int cnt = 0; cnt < 5; cnt++) {
				_IntegerManagerMap[col][row - cnt]->setRowCnt(NONE);
				_IntegerManagerMap[col][row - cnt]->setColCnt(NONE);
				_IntegerManagerMap[col][row - cnt]->setType(NONE);
			}
			for (int cnt = 0; cnt < 3; cnt++) {
				_IntegerManagerMap[col - cnt][row - 2]->setRowCnt(NONE);
				_IntegerManagerMap[col - cnt][row - 2]->setColCnt(NONE);
				_IntegerManagerMap[col - cnt][row - 2]->setType(NONE);
			}
			return true;
		}

		if (row + 2 > MAX_ROW_INDEX)
			throw IndexOutOfException();

		if (_IntegerManagerMap[col + 2][row - 2]->getColCnt() == 3 && _IntegerManagerMap[col + 2][row - 2]->getType() == type) {
			for (int cnt = 0; cnt < 5; cnt++) {
				_IntegerManagerMap[col][row - cnt]->setRowCnt(NONE);
				_IntegerManagerMap[col][row - cnt]->setColCnt(NONE);
				_IntegerManagerMap[col][row - cnt]->setType(NONE);
			}
			for (int cnt = 0; cnt < 3; cnt++) {
				_IntegerManagerMap[col + cnt][row - 2]->setRowCnt(NONE);
				_IntegerManagerMap[col + cnt][row - 2]->setColCnt(NONE);
				_IntegerManagerMap[col + cnt][row - 2]->setType(NONE);
			}
			return true;
		}
	}
	catch (IndexOutOfException e) {}
	return false;
}

void AnipangMap::straightenOut_checkMapValue() throw (IndexOutOfException) { // 카운트값, 타입 지정 함수 , CheckMap 포인터지정.
	
	AnipangManager& _manager = AnipangManager::instance();
	_IntegerManagerMap[0][0] = make_shared<CheckMap>(1, 1, 1);
	_IntegerManagerMap[0][1] = make_shared<CheckMap>(1, 2, 1);
	_IntegerManagerMap[0][2] = make_shared<CheckMap>(1, 1, 2);
	_IntegerManagerMap[0][3] = make_shared<CheckMap>(1, 1, 1);
	_IntegerManagerMap[0][4] = make_shared<CheckMap>(1, 2, 1);
	_IntegerManagerMap[0][5] = make_shared<CheckMap>(1, 1, 3);
	_IntegerManagerMap[0][6] = make_shared<CheckMap>(1, 2, 3);

	_IntegerManagerMap[1][0] = make_shared<CheckMap>(1, 1, 2);
	_IntegerManagerMap[1][1] = make_shared<CheckMap>(1, 2, 2);
	_IntegerManagerMap[1][2] = make_shared<CheckMap>(1, 1, 1);
	_IntegerManagerMap[1][3] = make_shared<CheckMap>(1, 1, 2);
	_IntegerManagerMap[1][4] = make_shared<CheckMap>(1, 1, 4);
	_IntegerManagerMap[1][5] = make_shared<CheckMap>(1, 1, 5);
	_IntegerManagerMap[1][6] = make_shared<CheckMap>(1, 1, SWORD);

	for (int col = 2; col <= MAX_COL_INDEX; col++){ // 3줄이상 나오는 애들 처음에 없게 !!
		for (int row = MIN_ROW_INDEX; row <= MAX_ROW_INDEX; row++) {
			int colCnt = 1, rowCnt = 1; 
			_IntegerManagerMap[col][row] = make_shared<CheckMap>(rowCnt, colCnt,_manager.getAnipangFactory()->getRandomType());
			try {
				if (col - 1 < MIN_COL_INDEX)
					throw IndexOutOfException();
				if (_IntegerManagerMap[col - 1][row]->getType() == _IntegerManagerMap[col][row]->getType() && _IntegerManagerMap[col - 1][row]->getColCnt() == 2 ){
					 _IntegerManagerMap[col][row]= nullptr; row--; continue;
				}
				else if (_IntegerManagerMap[col - 1][row]->getType() == _IntegerManagerMap[col][row]->getType())
					colCnt = _IntegerManagerMap[col - 1][row]->getColCnt() + 1;
			}
			catch (IndexOutOfException e) {}
			try {
				if (row - 1 < MIN_ROW_INDEX)
					throw IndexOutOfException();
				if (_IntegerManagerMap[col][row - 1]->getType() == _IntegerManagerMap[col][row]->getType() && _IntegerManagerMap[col][row - 1]->getRowCnt() == 2) {
					_IntegerManagerMap[col][row] = nullptr ; row--; continue;
				}
				else if (_IntegerManagerMap[col][row - 1]->getType() == _IntegerManagerMap[col][row]->getType())
					rowCnt = _IntegerManagerMap[col][row - 1]->getRowCnt() + 1;
			}
			catch (IndexOutOfException e) {}
			_IntegerManagerMap[col][row]->setColCnt(colCnt); _IntegerManagerMap[col][row]->setRowCnt(rowCnt);
		}
	}
}
void AnipangMap::produceNewFactor(){
	AnipangManager& _manager = AnipangManager::instance();
	_manager.getAnipangFactory()->produceNewFactor();
}
FindUsingBingo* AnipangMap::getRandomEffectBingoVector()
{
	return _randomBingoStore[getStoreIndex()];
}
void AnipangMap::checkAndSetIntegerBingo() {
	for (int bingo = 5; bingo >= 3; bingo--) { // 5 4 3 빙고
		for (int col = MAX_COL_INDEX; col >= 0; col--) {
			for (int row = MAX_ROW_INDEX; row >= 0; row--) {
				if (_IntegerManagerMap[col][row]->getColCnt() == bingo) { // 해당 타일의 컬럼값이 빙고 값와 같다면
					setIntegerTypeAndCheckSpecialType(col, row, bingo, true);
				}
				if (_IntegerManagerMap[col][row]->getRowCnt() == bingo) {
					setIntegerTypeAndCheckSpecialType(col, row, bingo, false);
				}
			}
		}
	}
}
