#pragma once

#include <vector>
#include "Dice.h"
#include "DiceFace.h"

class DiceSet
{
private:
	std::vector <Dice> dice;
public:
	DiceSet(); //Defaultni konstrikto-inicijalizira 6 kockica
	void roll_all();
	void roll_selected(const std::vector<int>& indeksi);
	std::vector<DiceFace> get_lica()const; //sva lica
	DiceFace get_lice_at(int idx)const;// lice jedne kockice

};
