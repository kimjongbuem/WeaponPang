#pragma once
class AnipangManager; // 일단 넣어둠... 타이머 객체를 한번생각해봄...
class AnipangGameScene; //reset 메서드를 통해 씬을 리셋 해야함..
class AnipangAction;
class Effect;
class ComboStore;
class AnipangMap{
public:
	AnipangMap();
	~AnipangMap();
public:
	void init(AnipangGameScene* scene);
	void setting();
	Sprite* getBackgroundNode();
	void straightenOut_checkMapValue() throw (IndexOutOfException);
	void produceNewFactor();
private:
	cocos2d::Sprite* background;
	weak_ptr<AnipangAction> _action;
	unique_ptr<Effect> _effect;
	vector<FindUsingBingo*> _randomBingoStore;
	vector<LumpPuzzle*> _lumps;
	int storeIndex = -1;
public:
	IntegerManagerMap _IntegerManagerMap;
	WeaponMap _checkWeaponMap;
	WeaponMap _newProduceMap;
public:
	int getStoreIndex() { return storeIndex; }
	FindUsingBingo* getRandomEffectBingoVector();
	void checkAndSetIntegerBingo(); // CheckMap에대한 설정
	void updateIntegerCheckMapRowAndColCnt();
	void convertMap();
private: // deleteable method
	void checkUpperFindingBingo(int cnt2Col, int cnt2Row, int alone_col, int alone_row);
	void checkDownFindingBingo(int cnt2Col, int cnt2Row, int alone_col, int alone_row);
	void checkRightFindingBingo(int cnt2Col, int cnt2Row, int alone_col, int alone_row);
	void checkLeftFindingBingo(int cnt2Col, int cnt2Row, int alone_col, int alone_row);
	void checkShapeVFindingBingo(int cnt2Col, int cnt2Row);
	void checkShapeReverseVFindingBingo(int cnt2Col, int cnt2Row);
	void checkInequlityBigRightFindingBingo(int cnt2Col, int cnt2Row);
	void checkInequlityBigLeftFindingBingo(int cnt2Col, int cnt2Row);
	void checkUpperRightFindingBingo(int cnt2Col, int cnt2Row, int alone_col, int alone_row, bool);
	void checkUpperLeftFindingBingo(int cnt2Col, int cnt2Row, int alone_col, int alone_row, bool);
	void checkDownRightFindingBingo(int cnt2Col, int cnt2Row, int alone_col, int alone_row, bool);
	void checkDownLeftFindingBingo(int cnt2Col, int cnt2Row, int alone_col, int alone_row, bool);

	bool getRandomShowingBingoVectorIndex();
private: // special block method
	bool checkGettingNoneType();
	void setIntegerTypeAndCheckSpecialType(int col, int row, int bingo, bool isCol);
	void deleteAroundSpecialBlockIsColBingoTrue(int col, int row);
	void deleteAroundSpecialBlockIsRowBingoTrue(int col, int row);
	bool deleteAroundSpecialBlockIsCol5BingoTrue(int col, int row);
	bool deleteAroundSpecialBlockIsRow5BingoTrue(int col, int row);
	bool deleteAroundSpecialBlockIsCol4BingoTrue(int col, int row);
	bool deleteAroundSpecialBlockIsCol3BingoTrue(int col, int row);
	bool deleteAroundSpecialBlockIsRow4BingoTrue(int col, int row);
	bool deleteAroundSpecialBlockIsRow3BingoTrue(int col, int row);
	void deleteAround4BingoRowSpecialValue(int col, int row, int sub, int type);
	void deleteAround4BingoColSpecialValue(int col, int row, int sub, int type);
public:
	void boomSpecialBingo(int col, int row); // 해당 스페셜 퍼즐이 삭제시 해당 스페셜 기준 3 * 3 폭발
public:
	bool checkDeletableBingo();// 유저가 일정 대기시간 초과시 효과를 통해 빙고를 찾게 도와주는 함수
	void showFindEffect(bool show);
	void turnoff_Effect();
	void initializeNewProduceMap();
	void checkMatching();
	void debugCCLOG() {
		for (int col = 0; col < 7; col++)
		{
			for (int row = 0; row < 7; row++) {
				CCLOG("%d : %d", col + 1, _IntegerManagerMap[col][row]->getType());
			}
		}
	}
public:
	void storeSpeicalBookIndex(int bookIndexY, int bookIndexX);
	int getSpeicalBookIndexX();
	int getSpeicalBookIndexY();
	int getCombo();
	void setCombo(int combo);
private:
	int specialBookX, specialBookY;
	ComboStore* _comboStore = nullptr;
public:
	void makeLump();
	void setLump(int col, int row);
	LumpPuzzle* getBigLumpPuzzle();
};
