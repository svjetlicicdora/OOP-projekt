#pragma once

#include "DiceFace.h"

class Dice {
private:
	DiceFace lice;	//trenutno
	static const DiceFace lica[];//sva moguæa

public:
	Dice(); 
	void roll();
	DiceFace get_lice()const;
};

