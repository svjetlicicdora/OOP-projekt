#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class UIGumb
{
public:
    UIGumb() = default;

    void postaviEkran(const sf::Vector2u& velicina);
    void postaviTekst(const std::string& t);

    // pozicija: donji desni kut (sa marginama)
    void postaviDonjeDesno(int redniBrojOdDesna);


    void postaviOmogucen(bool omogucen);
    bool jeOmogucen() const;

    bool jeKliknut(const sf::Vector2f& mis) const;

    void nacrtaj(sf::RenderWindow& prozor, const sf::Font& font) const;

private:
    sf::Vector2u velEkrana{ 1920u, 1080u };

    sf::FloatRect pravokutnik{ {0.f,0.f},{160.f,56.f} };
    std::string tekst = "GUMB";
    bool omogucen = true;

    float margina = 30.f;
    float razmak = 18.f;

private:
    bool tockaUPravokutniku(const sf::Vector2f& p, const sf::FloatRect& r) const;
};
