#include "Player.h"
#include <stdexcept>

void Player::takeDamage(int amount)
{
	if (amount > lp)
		lp = 0;
	else
		lp=lp-amount;
}

void Player::heal(int amount) 
{
	lp += amount;
	if (lp > 12)
		lp = 12;
}

void Player::addVP(int amount)
{
	vp = vp + amount;
}
 
void Player::addEC(int amount)
{
	ec = ec + amount;
}

bool Player::isAlive() const
{ 
	if (lp == 0)
		return false;
	else
		return true;
}

std::string Player::get_ime()const{return name;}
int Player::get_vp()const { return vp;}
int Player::get_lp()const { return lp;}
int Player::get_ec()const { return ec;}

bool Player::mozePlatitiEC(int iznos) const
{
	return ec >= iznos;
}

void Player::potrosiEC(int iznos)
{
	if (iznos < 0)
		throw std::invalid_argument("Iznos za potrošnju energije ne smije biti negativan.");

	if (ec < iznos)
		throw std::runtime_error("Nemate dovoljno energije (EC) za ovu akciju.");

	ec -= iznos;
}
