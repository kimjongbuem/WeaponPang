#pragma once
class ComboStore {
private:
	int combo;
public:
	ComboStore() { combo = 0; }
	void setCombo(int combo) { this->combo = combo; }
	int getCombo() { return combo; }
};