#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class UIPocetniEkran
{
public:
    UIPocetniEkran() = default;

    void postaviEkran(const sf::Vector2u& velicina);
    void reset();

    // vraæa true ako je klik nešto promijenio
    bool obradiKlik(const sf::Vector2f& mis);

    void nacrtaj(sf::RenderWindow& prozor, const sf::Font& font) const;

    bool jeSpreman() const;
    int dohvatiBrojIgraca() const;         // 2-6
    std::string dohvatiImeHumana() const;  // odabrano ime
    bool jeKliknutZapocni(const sf::Vector2f& mis) const;
    bool jeKliknutPravila(const sf::Vector2f& mis) const;



private:
    struct Gumb
    {
        sf::FloatRect rect;
        std::string tekst;
        bool odabran = false;
        bool omogucen = true;
    };

    sf::Vector2u velEkrana{ 1920u,1080u };

    std::vector<Gumb> gumbiBrojIgraca; // 2..6
    std::vector<Gumb> gumbiImena;      // 6 imena (stupac)
    Gumb gumbZapocni;
    Gumb gumbPravila;


    int odabraniBroj = -1;      // 0 = nije odabrano
    int odabranoImeIdx = -1;   // -1 = nije odabrano

    void izracunajLayout();
    void nacrtajGumb(sf::RenderWindow& prozor, const sf::Font& font, const Gumb& g) const;

    static bool sadrzi(const sf::FloatRect& r, const sf::Vector2f& p)
    {
        return r.contains(p);
    }
};
