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
		CCLOG("잘못된 숫자 %d 인덱스 넘버입니다.", number);
	}
};