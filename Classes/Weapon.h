#pragma once
class WeaponType;
class CheckMap {
private:
	int rowCnt = 0, colCnt = 0;
	int weaponType;
public:
	CheckMap(int type) {
		CheckMap(1, 1, type);
	}
	CheckMap(int rCnt, int cCnt, int type) {
		rowCnt = rCnt;
		colCnt = cCnt;
		weaponType = type;
	}
	int getRowCnt() {
		return rowCnt;
	}
	int getColCnt() {
		return colCnt;
	}
	void setRowCnt(int rCnt) {
		rowCnt = rCnt;
	}
	void setColCnt(int cCnt) {
		colCnt = cCnt;
	}
	int getType() {
		return weaponType;
	}
	void setType(int type) {
		weaponType = type;
	}
};
class Weapon {
public:
	//cocos2d::Node* axis; //부모이며 해당 sprite의 중심축이라고 생각하자.<굳이 안해도? >
	cocos2d::Sprite* weaponSprite;
	unique_ptr<WeaponType> type;
};

class WeaponType {
public:
	virtual ~WeaponType() = 0 {};
	virtual const int& getType() = 0 {};
	void setType(int type) { this->type = type; }
	int type;
};

class Arrow : public WeaponType {
public:
	virtual const int& getType() { type = ARROW;  return type; };
};
class SpecialArrow : public Arrow {
private: 
	int sType = S_ARROW;
public:
	virtual const int& getType() { return sType; }
};

class Boots : public WeaponType {
public:
	virtual const int& getType() { type = BOOTS;  return type; };
};
class SpecialBoots : public Boots {
private:
	int sType = S_BOOTS;
public:
	virtual const int& getType() { return sType; }
};

class Ring : public WeaponType {
public:
	virtual const int& getType() { type = RING; return type; };
};
class SpecialRing : public Ring {
private:
	int sType = S_RING;
public:
	virtual const int& getType() { return sType; }
};

class Staff : public WeaponType {
public:
	virtual const int& getType() { type = STAFF; return type; };
};
class SpecialStaff : public Staff {
private:
	int sType = S_STAFF;
public:
	virtual const int& getType() { return sType; }
};

class Axe : public WeaponType {
public:
	virtual const int& getType() { type = AXE; return type; };
};
class SpecialAxe : public Axe {
private:
	int sType = S_AXE;
public:
	virtual const int& getType() { return sType; }
};

class Belt : public WeaponType {
public:
	virtual const int& getType() { type = BELT; return type; };
};
class SpecialBelt : public Belt {
private:
	int sType = S_BELT;
public:
	virtual const int& getType() { return sType; }
};


class Hat : public WeaponType {
public:
	virtual const int& getType() { type = HAT; return type; };
};
class SpecialHat : public Hat {
private:
	int sType = S_HAT;
public:
	virtual const int& getType() { return sType; }
};

class Book : public WeaponType {
	virtual const int& getType() { type = BOOK; return type; }
};

class Sword : public WeaponType {
	virtual const int& getType() { type = SWORD; return type; }
};
typedef array<array<shared_ptr<CheckMap>, MAX_ROW>, MAX_COL> IntegerManagerMap;
typedef array<array<shared_ptr<Weapon>, MAX_ROW>, MAX_COL> WeaponMap;