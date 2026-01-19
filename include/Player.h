#pragma once
#include <string>


/*
lp=LIFE POINTS
vp=VICTORY POINTS
ec=ENERGY CUBES */

class Player {
private:
	std::string name;
	int lp = 10;
	int vp = 0;
	int ec = 0;
public:

	Player(std::string n) : name(n) {}
	void takeDamage(int amount);
	void heal(int amount);
	void addVP(int amount);
	void addEC(int amount);
	bool isAlive() const;
	std::string get_ime()const;
	int get_vp()const;
	int get_lp()const;
	int get_ec()const;
	bool mozePlatitiEC(int iznos) const;
	void potrosiEC(int iznos);

};

