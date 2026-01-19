#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class UIDeck
{
public:
    UIDeck() = default;

    // postavi poziciju i velièinu (zove UIManager)
    void postaviEkran(const sf::Vector2u& velicina);

    // stanje: nema kupljene kartice
    void prikaziDeck();

    // stanje: kupljena kartica
    void prikaziKartu(const std::string& naziv, const std::string& opis);

    // crtanje
    void nacrtaj(sf::RenderWindow& prozor, const sf::Font& font) const;

private:
    sf::FloatRect rect;

    bool imaKartu = false;
    std::string nazivKarte;
    std::string opisKarte;

    void nacrtajOkvir(sf::RenderWindow& prozor) const;
};
