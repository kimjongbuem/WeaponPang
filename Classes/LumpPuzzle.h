#pragma once
class LumpPuzzle {
private:
	int startRow, startCol; // 해당 row, col인덱스..
	int endRow; // startCol이 해당 col index 이기 때문임 ^^ 
	int cnt; // 덩어리 개수
public:
	LumpPuzzle(int col, int row, int cnt, int endRow) {
		this->startCol = col;
		this->startRow = row;
		this->cnt = cnt;
		this->endRow = endRow;
	}
	int getEndRowIndex() {
		return endRow;
	}
	int getLumpCnt() {
		return cnt;
	}
	void setLumpCnt(int cnt) {
		this->cnt = cnt;
	}
	int getStartRowIndex() {
		return startRow;
	}
	int getStartColIndex() {
		return startCol;
	}
};