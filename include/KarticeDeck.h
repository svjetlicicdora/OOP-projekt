#pragma once
#include <vector>
#include <memory>
#include <random>

#include "PowerKartice.h"

class KarticeDeck
{
private:
    std::vector<std::unique_ptr<PowerKartice>> karte;
    std::mt19937 rng;

public:
    KarticeDeck();

    bool prazno() const { return karte.empty(); }
    int brojKarata() const { return static_cast<int>(karte.size()); }

    // Izvuci random kartu (i makni je iz decka)
    std::unique_ptr<PowerKartice> izvuciRandom();
};