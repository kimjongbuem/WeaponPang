#pragma once
class IndexOutOfException {
private:
	int number;
public:
	IndexOutOfException() {
		
	}
	IndexOutOfException(int number) {
		this->number = number;
	}
	void showIndexOfOutExceptionMessage() {
		CCLOG("�߸��� ���� %d �ε��� �ѹ��Դϴ�.", number);
	}
};