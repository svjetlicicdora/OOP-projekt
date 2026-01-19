#pragma once

#include "PowerKartice.h"

class KartaSlava :public PowerKartice
{
public:
	int cijena() const override
	{
		return 3;
	}

	std::string naziv() const override
	{
		return "Karta Slave";
	}

	std::string opis() const override
	{
		return "+2VP";
	}

	void primijeni(Game& game, Player& player) override;
};


class KartaTrofej :public PowerKartice
{
public:
	int cijena () const override
	{
		return 3;
	}
	std::string naziv() const override
	{
		return "Karta Trofej";
	}
	std::string opis() const override
	{
		return "return +1VP";
	}
	void primijeni(Game& game, Player& player) override;
};


class KartaGenerator :public PowerKartice
{
	public:
	int cijena() const override
	{
		return 3;
	}
	std::string naziv() const override
	{
		return "Karta Generator";
	}
	std::string opis() const override
	{
		return "+1 Energy";
	}
	void primijeni(Game& game, Player& player) override;
};

class KartaBaterija :public PowerKartice
{
	public:
	int cijena() const override
	{
		return 2;
	}
	std::string naziv() const override
	{
		return "Karta Baterija";
	}
	std::string opis() const override
	{
		return "+2 Energy";
	}
	void primijeni(Game& game, Player& player) override;
};


class KartaPrvaPomoc :public PowerKartice
{
public:
	int cijena() const override
	{
		return 3;
	}
	std::string naziv() const override
	{
		return "Prva pomoæ";
	}
	std::string opis() const override
	{
		return "+2 srca ako nisi u Tokyu";
	}

	void primijeni(Game& game, Player& player) override;
};

class KartaRegeneracija :public PowerKartice
{
	public:
	int cijena() const override
	{
		return 3;
	}
	std::string naziv() const override
	{
		return "Karta Regeneracija";
	}
	std::string opis() const override
	{
		return "+1 srce ako nisi u Tokyu";
	}
	void primijeni(Game& game, Player& player) override;
};

class KartaJackpot :public PowerKartice
{
public:
	int cijena() const override
	{
		return 3;
	}
	std::string naziv() const override
	{
		return "Karta Jackpot";
	}
	std::string opis() const override
	{
		return "+1 VP i +2 Energy";
	}
	void primijeni(Game& game, Player& player) override;
};

class KartaNagrada :public PowerKartice
{
	public:
	int cijena() const override
	{
		return 3;
	}
	std::string naziv() const override
	{
		return "Karta Nagrada";
	}
	std::string opis() const override
	{
		return "+3 VP";
	}

	void primijeni(Game& game, Player& player) override;
};

class KartaSuperPunjenje	:public PowerKartice
{
	public:
	int cijena() const override
	{
		return 3;
	}
	std::string naziv() const override
	{
		return "Karta Super Punjenje";
	}
	std::string opis() const override
	{
		return "+5 Energy";
	}
	void primijeni(Game& game, Player& player) override;
};

class KartaMalaNagrada :public PowerKartice
{
	public:
	int cijena() const override
	{
		return 3;
	}
	std::string naziv() const override
	{
		return "Karta Mala Nagrada";
	}
	std::string opis() const override
	{
		return "+1 VP i +1 Energy";
	}
	void primijeni(Game& game, Player& player) override;
};