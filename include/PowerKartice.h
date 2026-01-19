#pragma once
#include "KarticeCijena.h"
#include "KarticeUI.h"

class Game;
class Player;

class PowerKartice : public KarticeUI, public KarticeCijena
{
public:
	virtual ~PowerKartice() = default;
	virtual void primijeni(Game& game, Player& player) = 0;
};