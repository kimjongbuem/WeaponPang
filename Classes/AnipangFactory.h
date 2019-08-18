#pragma once
class AnipangManager;
class AnipangMap; // 왜냐하면 맵을 통해 넣고 백그라운드 이미지가 부모 스프라이트 이기때문에 맵이 필요함.

class AnipangFactory {
public:
	AnipangFactory();
	~AnipangFactory();
	void settingMap();
	int getRandomType() throw(IndexOutOfException);
	void produceNewFactor();
private:
	void setEmptyColCnt(int);
	int getEmptyColCnt(int);
	void initializeEmptyColCntZero();
	void setTempProducingMap(int row);
	void dropNewProducing(int row);
	void dropNewProducingSetVisible(int col, int row);
public:
	void setWeaponSprite(shared_ptr<Weapon> weapon, int type);
	unique_ptr<WeaponType> setWeaponType(int);
private:
	array<int, 7> _emptyColCnt;
	shared_ptr<AnipangMap> _map;
};