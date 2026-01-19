#include "GameOverEkran.h"

void GameOverEkran::postaviEkran(const sf::Vector2u& velicina)
{
    velEkrana = velicina;
    izracunajLayout();
}

void GameOverEkran::reset()
{
    klikNovaIgra = false;
    klikIzlaz = false;
    blinkSat.restart();
    blinkVidljivo = true;
}

void GameOverEkran::izracunajLayout()
{
    float W = (float)velEkrana.x;
    float H = (float)velEkrana.y;

    float gW = 260.f;
    float gH = 70.f;
    float razmak = 30.f;

    float cx = W / 2.f;
    float y = H / 2.f + 80.f;

    rectNovaIgra = sf::FloatRect({ cx - gW - razmak / 2.f, y }, { gW, gH });
    rectIzlaz = sf::FloatRect({ cx + razmak / 2.f, y }, { gW, gH });
}

bool GameOverEkran::obradiKlik(const sf::Vector2f& mis)
{
    klikNovaIgra = false;
    klikIzlaz = false;

    if (rectNovaIgra.contains(mis))
    {
        klikNovaIgra = true;
        return true;
    }

    if (rectIzlaz.contains(mis))
    {
        klikIzlaz = true;
        return true;
    }

    return false;
}

bool GameOverEkran::jeKliknutaNovaIgra() const
{
    return klikNovaIgra;
}

bool GameOverEkran::jeKliknutIzlaz() const
{
    return klikIzlaz;
}

void GameOverEkran::nacrtaj(sf::RenderWindow& prozor, const sf::Font& font)
{
    // blink svake 0.6 sekundi
    if (blinkSat.getElapsedTime().asSeconds() > 0.6f)
    {
        blinkVidljivo = !blinkVidljivo;
        blinkSat.restart();
    }

    float cx = velEkrana.x / 2.f;
    float cy = velEkrana.y / 2.f - 80.f;

    if (blinkVidljivo)
    {
        // GLOW
        sf::Text glow(font, poruka, 84);
        glow.setFillColor(sf::Color::Transparent);
        glow.setOutlineThickness(6.f);
        glow.setOutlineColor(sf::Color(255, 200, 0));

        auto gb = glow.getLocalBounds();
        glow.setOrigin({ gb.position.x + gb.size.x / 2.f,
                         gb.position.y + gb.size.y / 2.f });
        glow.setPosition({ cx, cy });
        prozor.draw(glow);

        // TEKST
        sf::Text txt(font, poruka, 84);
        txt.setFillColor(sf::Color::White);

        auto b = txt.getLocalBounds();
        txt.setOrigin({ b.position.x + b.size.x / 2.f,
                        b.position.y + b.size.y / 2.f });
        txt.setPosition({ cx, cy });
        prozor.draw(txt);

    }

    // gumbi
    nacrtajGumb(prozor, font, rectNovaIgra, "NOVA IGRA");
    nacrtajGumb(prozor, font, rectIzlaz, "IZAÐI");
}

void GameOverEkran::nacrtajGumb(sf::RenderWindow& prozor,
    const sf::Font& font,
    const sf::FloatRect& rect,
    const std::string& tekst) const
{
    sf::RectangleShape box(rect.size);
    box.setPosition(rect.position);
    box.setFillColor(sf::Color(45, 45, 45));
    box.setOutlineThickness(3.f);
    box.setOutlineColor(sf::Color::White);
    prozor.draw(box);

    sf::Text t(font, tekst, 26);
    t.setFillColor(sf::Color::White);

    auto b = t.getLocalBounds();
    t.setOrigin({ b.position.x + b.size.x / 2.f,
                  b.position.y + b.size.y / 2.f });
    t.setPosition({
        rect.position.x + rect.size.x / 2.f,
        rect.position.y + rect.size.y / 2.f
        });
    prozor.draw(t);
}
