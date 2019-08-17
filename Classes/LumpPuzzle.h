#pragma once
class LumpPuzzle {
private:
	int startRow, startCol; // �ش� row, col�ε���..
	int endRow; // startCol�� �ش� col index �̱� ������ ^^ 
	int cnt; // ��� ����
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