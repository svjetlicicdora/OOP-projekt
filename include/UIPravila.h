#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class UIPravila
{
public:
    UIPravila();

    // uèitava tekst iz assets/pravila.txt
    void ucitajDatoteku();

    // crta cijeli ekran pravila
    void nacrtaj(sf::RenderWindow& prozor, sf::Font& font);

    // vraæa true ako je kliknut gumb NATRAG
    bool obradiKlik(const sf::Vector2f& mis);

private:
    std::string tekst;        // sadržaj pravila

    // pozicije i stilovi
    sf::FloatRect rectNatrag; // podruèje gumba NATRAG
};