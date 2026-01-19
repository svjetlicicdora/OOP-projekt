#include "UIDeckPanel.h"

void UIDeck::postaviEkran(const sf::Vector2u& vel)
{
    // gornji desni kut
    float sirina = 340.f;
    float visina = 180.f;


    //pozicioniranje deck pravokutnika na ekranu
    rect = sf::FloatRect(
        sf::Vector2f(vel.x - sirina - 40.f, 150.f),
        sf::Vector2f(sirina, visina)
    );


}

void UIDeck::prikaziDeck()
{
    imaKartu = false;
    nazivKarte.clear();
    opisKarte.clear();
}

void UIDeck::prikaziKartu(const std::string& naziv, const std::string& opis)
{
    imaKartu = true;
    nazivKarte = naziv;
    opisKarte = opis;
}

void UIDeck::nacrtaj(sf::RenderWindow& prozor, const sf::Font& font) const
{
    nacrtajOkvir(prozor);

    if (!imaKartu)
    {
        // PRAZAN DECK 
        sf::Text t(font, "DECK", 28);
        t.setFillColor(sf::Color::White);

        auto b = t.getLocalBounds();
        t.setOrigin({ b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f });
        t.setPosition({
            rect.position.x + rect.size.x / 2.f,
            rect.position.y + rect.size.y / 2.f
            });

        prozor.draw(t);

        sf::Text hint(font, "Kupnja: 3 energije", 16);
        hint.setFillColor(sf::Color(200, 200, 200));
        hint.setPosition({
            rect.position.x + 20.f,
            rect.position.y + rect.size.y - 28.f
            });
        prozor.draw(hint);
    }
    else
    {
        //  KUPLJENA KARTICA 
        sf::Text naslov(font, nazivKarte, 22);
        naslov.setFillColor(sf::Color(255, 200, 0));
        naslov.setPosition({
            rect.position.x + 16.f,
            rect.position.y + 16.f
            });
        prozor.draw(naslov);

        sf::Text opis(font, opisKarte, 18);
        opis.setFillColor(sf::Color::White);
        opis.setPosition({
            rect.position.x + 16.f,
            rect.position.y + 52.f
            });
        prozor.draw(opis);
    }
}

void UIDeck::nacrtajOkvir(sf::RenderWindow& prozor) const
{
    sf::RectangleShape r({ rect.size.x, rect.size.y });
    r.setPosition(rect.position);
    r.setFillColor(sf::Color(35, 35, 35));
    r.setOutlineThickness(3.f);
    r.setOutlineColor(sf::Color::White);
    prozor.draw(r);
}
