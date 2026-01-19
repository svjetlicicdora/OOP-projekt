#pragma once
#include <SFML/Graphics.hpp>

class GameOverEkran
{
public:
    GameOverEkran() = default;

    void postaviEkran(const sf::Vector2u& velicina);
    void reset();

    // obrada klika – vraæa true ako je klik nešto pogodio
    bool obradiKlik(const sf::Vector2f& mis);

    void nacrtaj(sf::RenderWindow& prozor, const sf::Font& font);

    bool jeKliknutaNovaIgra() const;
    bool jeKliknutIzlaz() const;
    void postaviPoruku(const std::string& t) { poruka = t; }

private:
    sf::Vector2u velEkrana{ 1920,1080 };

    // blink efekt
    sf::Clock blinkSat;
    bool blinkVidljivo = true;

    // gumbi
    sf::FloatRect rectNovaIgra;
    sf::FloatRect rectIzlaz;

    bool klikNovaIgra = false;
    bool klikIzlaz = false;

    void izracunajLayout();
    void nacrtajGumb(sf::RenderWindow& prozor,
        const sf::Font& font,
        const sf::FloatRect& rect,
        const std::string& tekst) const;

    std::string poruka = "GAME OVER";
};
