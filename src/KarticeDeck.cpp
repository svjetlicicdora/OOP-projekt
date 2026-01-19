#include "KarticeDeck.h"
#include "Kartice10.h"

#include <stdexcept>

KarticeDeck::KarticeDeck()
    : rng(std::random_device{}())
{
  
    karte.push_back(std::make_unique<KartaSlava>());
    karte.push_back(std::make_unique<KartaTrofej>());
    karte.push_back(std::make_unique<KartaGenerator>());
    karte.push_back(std::make_unique<KartaBaterija>());
    karte.push_back(std::make_unique<KartaPrvaPomoc>());
    karte.push_back(std::make_unique<KartaRegeneracija>());
    karte.push_back(std::make_unique<KartaJackpot>());
    karte.push_back(std::make_unique<KartaNagrada>());
    karte.push_back(std::make_unique<KartaSuperPunjenje>());
    karte.push_back(std::make_unique<KartaMalaNagrada>());
}

std::unique_ptr<PowerKartice> KarticeDeck::izvuciRandom()
{
    if (karte.empty())
        throw std::runtime_error("Deck je prazan - nema više karata.");

    std::uniform_int_distribution<int> dist(0, static_cast<int>(karte.size()) - 1);
    int idx = dist(rng);

    
    std::unique_ptr<PowerKartice> odabrana = std::move(karte[idx]);
    karte[idx] = std::move(karte.back());
    karte.pop_back();

    return odabrana;
}
