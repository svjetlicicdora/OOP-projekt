#include "UIPravila.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <SFML/Graphics/Text.hpp>

UIPravila::UIPravila()
{
    // područje gumba NATRAG (fiksna pozicija)
    rectNatrag = sf::FloatRect(
        { 80.f, 1000.f },   // pozicija
        { 200.f, 60.f }     // širina, visina
    );
}

void UIPravila::ucitajDatoteku()
{
    std::ifstream file("assets/pravila.txt");
    if (!file)
        throw std::runtime_error("Ne mogu ucitati datoteku: assets/pravila.txt");

    tekst.assign(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );
}

void UIPravila::nacrtaj(sf::RenderWindow& prozor, sf::Font& font)
{
    prozor.clear(sf::Color::Black);

    // Naslov
    sf::Text naslov(font, "PRAVILA IGRE", 48);
    naslov.setFillColor(sf::Color::White);
    naslov.setPosition({ 80.f, 40.f });
    prozor.draw(naslov);

    // Tekst pravila
    sf::Text t(font, tekst, 26);
    t.setFillColor(sf::Color::White);
    t.setPosition({ 80.f, 140.f });
    prozor.draw(t);

    // Gumb NATRAG
    sf::Text back(font, "NATRAG", 32);
    back.setFillColor(sf::Color::White);
    back.setPosition(rectNatrag.position);
    prozor.draw(back);
}

bool UIPravila::obradiKlik(const sf::Vector2f& mis)
{
    if (rectNatrag.contains(mis))
        return true; // signal UIManageru da se vrati na početni ekran

    return false;
}