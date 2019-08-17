#pragma once
class BingoStoreIndex {
private:
	int rowIndex; int colIndex;
public:
	BingoStoreIndex(int colIndex, int rowIndex) : colIndex(colIndex), rowIndex(rowIndex){}
	int getRowIndex() {
		return rowIndex;
	}
	int getColIndex() {
		return colIndex;
	}
};

class FindUsingBingo {
private:
	BingoStoreIndex* _cnt2Obj;
	BingoStoreIndex* _cnt1Obj;
	BingoStoreIndex* _aloneObj;
public:
	FindUsingBingo(int cnt2ObjCol, int cnt2ObjRow, int cnt1ObjCol, int cnt1ObjRow, int aloneObjCol, int  aloneObjRow) {
		_cnt2Obj = new BingoStoreIndex(cnt2ObjCol, cnt2ObjRow);
		_aloneObj = new BingoStoreIndex(aloneObjCol, aloneObjRow);
		_cnt1Obj = new BingoStoreIndex(cnt1ObjCol, cnt1ObjRow);
	}
	BingoStoreIndex* getCnt2Obj() {
		return _cnt2Obj;
	}
	BingoStoreIndex* getAloneObj() {
		return _aloneObj;
	}
	BingoStoreIndex* getCnt1Obj() {
		return _cnt1Obj;
	}
};