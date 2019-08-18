#pragma once
class AnipangManager;
class AnipangMap; // �ֳ��ϸ� ���� ���� �ְ� ��׶��� �̹����� �θ� ��������Ʈ �̱⶧���� ���� �ʿ���.

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