#include "DiceSet.h"

// Konstruktor koji inicijalizira 6 kockica
DiceSet::DiceSet()
{
	for (int i = 0; i < 6; i++)
	{
		dice.push_back(Dice());
	}
}
// Bacanje svih kockica
void DiceSet::roll_all()
{
	for (auto& d : dice)
	{
		d.roll();
	}
}

// Bacanje odabranih kockica koje igraè nije oznaèio
void DiceSet::roll_selected(const std::vector<int>& indeksi)
{
	for (int idx : indeksi)
	{
		if (idx >= 0 && idx < dice.size())
		{
			dice[idx].roll();
		}
	}
}

// Vraæa lica svih kockica u obliku vektora
std::vector<DiceFace>DiceSet::get_lica()const
{
	std::vector<DiceFace> lica;
	for (const auto& d : dice)
	{
		lica.push_back(d.get_lice());
	}
	return lica;
}

// Vraæa lice kockice na odreðenom indeksu
DiceFace DiceSet::get_lice_at(int indeks)const
{
	return dice[indeks].get_lice();
}