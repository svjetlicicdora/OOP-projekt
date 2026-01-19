#pragma once
#include <string>

class KarticeUI
{
public:
	virtual ~KarticeUI() = default;
	virtual std::string naziv() const = 0;
	virtual std::string opis()const = 0;
}; 
