#include "Dice.h"
#include <cstdlib>
#include <ctime>

static bool seed = []() 
	{
		srand(static_cast<unsigned>(time(nullptr)));
		return true;
	}();

const DiceFace Dice::lica[7] = {
DiceFace::One,
DiceFace::Two,
DiceFace::Three,
DiceFace::Heart,
DiceFace::Lightning,
DiceFace::Claw,
};

Dice::Dice():lice(DiceFace::One){} //defaultno lice

void Dice::roll()
{
	int idx = rand() % 6;
	lice = lica[idx];
}

DiceFace Dice::get_lice()const
{
	return lice;
}
