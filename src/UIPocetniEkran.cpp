#include "UIPocetniEkran.h"


void UIPocetniEkran::postaviEkran(const sf::Vector2u& velicina)
{
    velEkrana = velicina;
    reset();
    izracunajLayout();
    
}

void UIPocetniEkran::reset()
{
    odabraniBroj = -1;
    odabranoImeIdx = -1;

    gumbiBrojIgraca.clear();
    gumbiImena.clear();

    // Brojevi igraèa (2–6)
    for (int n = 2; n <= 6; n++)
    {
        Gumb g;
        g.tekst = std::to_string(n);
        g.odabran = false;
        g.omogucen = true;
        gumbiBrojIgraca.push_back(g);
    }

    // Imena likova
    static const std::vector<std::string> imena = {
        "Cyber Kitty",
        "Gigazaur",
        "Alenoid",
        "Space Penguin",
        "The King",
        "Meka Dragon"
    };

    for (const auto& s : imena)
    {
        Gumb g;
        g.tekst = s;
        g.odabran = false;
        g.omogucen = true;
        gumbiImena.push_back(g);
    }

    // Gumb ZAPOÈNI
    gumbZapocni = Gumb{};
    gumbZapocni.tekst = "ZAPOÈNI";
    gumbZapocni.omogucen = false;

    // Gumb PRAVILA
    gumbPravila = Gumb{};
    gumbPravila.tekst = "PRAVILA";
    gumbPravila.omogucen = true;

    for (auto& g : gumbiBrojIgraca)
        g.odabran = false;

    for (auto& g : gumbiImena)
        g.odabran = false;

    gumbZapocni.odabran = false;
    gumbPravila.odabran = false;
}
bool UIPocetniEkran::jeSpreman() const
{
    return (odabraniBroj >= 2 && odabraniBroj <= 6 && odabranoImeIdx >= 0 && odabranoImeIdx < (int)gumbiImena.size());
}

int UIPocetniEkran::dohvatiBrojIgraca() const
{
    return odabraniBroj;
}

std::string UIPocetniEkran::dohvatiImeHumana() const
{
    if (odabranoImeIdx < 0 || odabranoImeIdx >= (int)gumbiImena.size()) return "";
    return gumbiImena[odabranoImeIdx].tekst;
}

void UIPocetniEkran::izracunajLayout()
{
    const float W = (float)velEkrana.x;
    const float H = (float)velEkrana.y;

    const float gW = 70.f;
    const float gH = 55.f;
    const float razmak = 16.f;

    // Broj igraèa — centrirano
    float ukupnaSirina = gumbiBrojIgraca.size() * gW + (gumbiBrojIgraca.size() - 1) * razmak;
    float xBroj = (W - ukupnaSirina) / 2.f;
    float yBroj = 160.f;

    for (int i = 0; i < (int)gumbiBrojIgraca.size(); i++)
    {
        gumbiBrojIgraca[i].rect = sf::FloatRect(
            { xBroj + i * (gW + razmak), yBroj },
            { gW, gH }
        );
    }

    // Imena — centrirano
    const float imeW = 420.f;
    const float imeH = 58.f;
    const float imeRazmak = 14.f;

    float xIme = (W - imeW) / 2.f;
    float yIme = yBroj + 120.f;

    for (int i = 0; i < (int)gumbiImena.size(); i++)
    {
        gumbiImena[i].rect = sf::FloatRect(
            { xIme, yIme + i * (imeH + imeRazmak) },
            { imeW, imeH }
        );
    }

    // ZAPOÈNI
    float zW = 240.f;
    float zH = 64.f;
    float xZ = (W - zW) / 2.f;
    float yZ = yIme + (float)gumbiImena.size() * (imeH + imeRazmak) + 40.f;

    gumbZapocni.rect = sf::FloatRect({ xZ, yZ }, { zW, zH });
    gumbZapocni.omogucen = jeSpreman();

    // PRAVILA
    float pW = zW;
    float pH = zH;
    float xP = xZ;
    float yP = yZ + zH + 20.f;

    gumbPravila.rect = sf::FloatRect({ xP, yP }, { pW, pH });
}
bool UIPocetniEkran::obradiKlik(const sf::Vector2f& mis)
{
    // Broj igraèa
    for (int i = 0; i < (int)gumbiBrojIgraca.size(); i++)
    {
        if (sadrzi(gumbiBrojIgraca[i].rect, mis))
        {
            for (auto& g : gumbiBrojIgraca)
                g.odabran = false;

            gumbiBrojIgraca[i].odabran = true;
            odabraniBroj = 2 + i;
            gumbZapocni.omogucen = jeSpreman();
            return true;
        }
    }

    // Imena
    for (int i = 0; i < (int)gumbiImena.size(); i++)
    {
        if (sadrzi(gumbiImena[i].rect, mis))
        {
            for (auto& g : gumbiImena)
                g.odabran = false;

            gumbiImena[i].odabran = true;
            odabranoImeIdx = i;
            gumbZapocni.omogucen = jeSpreman();
            return true;
        }
    }

    // ZAPOÈNI
    if (sadrzi(gumbZapocni.rect, mis))
        return true;

    // PRAVILA
    if (sadrzi(gumbPravila.rect, mis))
        return true;

    return false;
}
void UIPocetniEkran::nacrtaj(sf::RenderWindow& prozor, const sf::Font& font) const
{
  
    // naslov u gornjem desnom uglu

    {
        sf::Text naslov(font, "KING OF TOKYO", 44);
        naslov.setFillColor(sf::Color::White);
        auto b = naslov.getLocalBounds();
        naslov.setOrigin({ b.position.x + b.size.x, b.position.y });
        naslov.setPosition({ (float)velEkrana.x - 60.f, 40.f });
        prozor.draw(naslov);
    }

    // podnaslovi
    {
        sf::Text t1(font, "Odaberi broj igraèa (2-6):", 28);
        t1.setFillColor(sf::Color::White);
        t1.setPosition({ 80.f, 170.f });
        prozor.draw(t1);

        sf::Text t2(font, "Odaberi lika (Human):", 28);
        t2.setFillColor(sf::Color::White);
        t2.setPosition({ 80.f, 300.f });
        prozor.draw(t2);
    }

    // gumbi broj igraèa
    for (const auto& g : gumbiBrojIgraca)
        nacrtajGumb(prozor, font, g);

    // gumbi imena
    for (const auto& g : gumbiImena)
        nacrtajGumb(prozor, font, g);

    // gumb zapoèni
    nacrtajGumb(prozor, font, gumbZapocni);
    nacrtajGumb(prozor, font, gumbPravila);


    // mala uputa
    {
        sf::Text uputa(font, "Klikni ZAPOÈNI kad odabereš broj igraèa i lika.", 20);
        uputa.setFillColor(sf::Color(220, 220, 220));
        uputa.setPosition({ 80.f, (float)velEkrana.y - 80.f });
        prozor.draw(uputa);
    }
}

void UIPocetniEkran::nacrtajGumb(sf::RenderWindow& prozor, const sf::Font& font, const Gumb& g) const
{
    sf::RectangleShape r(g.rect.size);
    r.setPosition(g.rect.position);

    if (!g.omogucen)
    {
        r.setFillColor(sf::Color(35, 35, 35));
    }
    else if (g.odabran)
    {
        r.setFillColor(sf::Color(80, 80, 80)); 
    }
    else
    {
        r.setFillColor(sf::Color(55, 55, 55));
    }

    r.setOutlineThickness(2.f);
    r.setOutlineColor(sf::Color::White);

    prozor.draw(r);
    // glow/istakni odabrano
    if (g.odabran)
    {
        sf::RectangleShape glow({ g.rect.size.x + 10.f, g.rect.size.y + 10.f });
        glow.setOrigin({ 5.f,5.f });
        glow.setPosition(g.rect.position);
        glow.setFillColor(sf::Color::Transparent);
        glow.setOutlineThickness(4.f);
        glow.setOutlineColor(sf::Color(255, 200, 0));
        prozor.draw(glow);
    }


    sf::Text txt(font, g.tekst, 22);
    txt.setFillColor(g.omogucen ? sf::Color::White : sf::Color(160, 160, 160));
    auto b = txt.getLocalBounds();
    txt.setOrigin({ b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f });
    txt.setPosition({ g.rect.position.x + g.rect.size.x / 2.f, g.rect.position.y + g.rect.size.y / 2.f });
    prozor.draw(txt);
}

bool UIPocetniEkran::jeKliknutZapocni(const sf::Vector2f& mis) const
{
    return gumbZapocni.omogucen && gumbZapocni.rect.contains(mis);
}

bool UIPocetniEkran::jeKliknutPravila(const sf::Vector2f& mis) const
{
    return gumbPravila.rect.contains(mis);
}
