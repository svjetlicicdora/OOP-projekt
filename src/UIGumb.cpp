#include "UIGumb.h"

void UIGumb::postaviEkran(const sf::Vector2u& velicina)
{
    velEkrana = velicina;
}

void UIGumb::postaviTekst(const std::string& t)
{
    tekst = t;
}

void UIGumb::postaviDonjeDesno(int redniBrojOdDesna)
{
    const float w = pravokutnik.size.x;
    const float h = pravokutnik.size.y;

    float x = (float)velEkrana.x - margina - w - (float)redniBrojOdDesna * (w + razmak);
    float y = (float)velEkrana.y - margina - h;

    pravokutnik.position = { x, y };
}

void UIGumb::postaviOmogucen(bool o)
{
    omogucen = o;
}

bool UIGumb::jeOmogucen() const
{
    return omogucen;
}

bool UIGumb::jeKliknut(const sf::Vector2f& mis) const
{
    if (!omogucen) return false;
    return tockaUPravokutniku(mis, pravokutnik);
}

bool UIGumb::tockaUPravokutniku(const sf::Vector2f& p, const sf::FloatRect& r) const
{
    return (p.x >= r.position.x &&
        p.x <= r.position.x + r.size.x &&
        p.y >= r.position.y &&
        p.y <= r.position.y + r.size.y);
}

void UIGumb::nacrtaj(sf::RenderWindow& prozor, const sf::Font& font) const
{
    sf::RectangleShape okvir(pravokutnik.size);
    okvir.setPosition(pravokutnik.position);

    if (omogucen)
        okvir.setFillColor(sf::Color(35, 35, 35));
    else
        okvir.setFillColor(sf::Color(20, 20, 20));

    okvir.setOutlineThickness(2.f);
    okvir.setOutlineColor(sf::Color::White);
    prozor.draw(okvir);

    sf::Text t(font, tekst, 22);
    t.setFillColor(omogucen ? sf::Color::White : sf::Color(140, 140, 140));

    auto b = t.getLocalBounds();
    t.setOrigin({ b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f });
    t.setPosition({
        pravokutnik.position.x + pravokutnik.size.x / 2.f,
        pravokutnik.position.y + pravokutnik.size.y / 2.f
        });

    prozor.draw(t);
}
