#pragma once

#include <vector>
#include "DiceFace.h"

//Foward deklaracije da ne moram includeat cijele klase

class Game;
class Player;

//apstraktna klasa koja definira sucelje za kontrolere

class IController
{
public:
	virtual ~IController() = default;
	
	virtual std::vector<int> odaberiKockicu(
		const Game& game,
		const Player& me,
		const std::vector<DiceFace>& currentFaces,
		int rollsLeft
	) = 0;


	virtual bool decideYield(
		const Game& game,
		const Player& me,
		int incomingDamage
	) = 0;
};