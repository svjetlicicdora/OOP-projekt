#include "UIKockice.h"
#include <algorithm>
#include <iostream>

void UIKockice::postaviEkran(const sf::Vector2u& velicina)
{
    velEkrana = velicina;
    izracunajLayout();
}

bool UIKockice::ucitajIkone(const std::string& folder)
{
    //učitava ikone iz assets foldera
    bool ok1 = texHeart.loadFromFile(folder + "/heart.png");
    bool ok2 = texClaw.loadFromFile(folder + "/claw.png");
    bool ok3 = texEnergy.loadFromFile(folder + "/lightning.png");

    ikoneUcitan = ok1 && ok2 && ok3;
    return ikoneUcitan;
}

void UIKockice::postaviKockice(const std::vector<UIKockica>& nove)
{
    std::cout << "postaviKockice pozvano, size=" << nove.size() << "\n";
    // PRAZNE kockice
    if (nove.empty())
    {
        // postavi 6 praznih kockica
        kockice.clear();
        kockice.resize(6);

        for (auto& k : kockice)
        {
            k.vrijednost = UIKockicaVrijednost::One;
            k.prazna = true;                         
            k.zadrzana = false;
        }

        izracunajLayout();
        return;
    }

    if (kockice.empty())
    {
        kockice = nove;
        izracunajLayout();
        return;
    }

    int n = (int)std::min(kockice.size(), nove.size());
    for (int i = 0; i < n; i++)
    {
        if (!kockice[i].zadrzana)
        {
            kockice[i].vrijednost = nove[i].vrijednost;
            kockice[i].prazna = false; 
        }
    }

    if (kockice.size() != nove.size())
    {
        kockice.resize(nove.size());
        for (int i = n; i < (int)nove.size(); i++)
            kockice[i] = nove[i];

        izracunajLayout();
    }
}


void UIKockice::postaviPreostalaBacanja(int br)
{
    preostalaBacanja = br;
}

void UIKockice::izracunajLayout()
{
    rects.clear();

    // donji lijevi kut, malo iznad dna
    float y = (float)velEkrana.y - 170.f;
    pocetak.y = y;

    rects.reserve(kockice.size());
    for (int i = 0; i < (int)kockice.size(); i++)
    {
        float x = pocetak.x + i * (velicinaKockice + razmak);
        rects.emplace_back(
            sf::Vector2f(x, y),
            sf::Vector2f(velicinaKockice, velicinaKockice)
        );
    }
}

bool UIKockice::obradiKlik(const sf::Vector2f& mis)
{
    for (int i = 0; i < (int)rects.size(); i++)
    {
        if (rects[i].contains(mis))
        {
            kockice[i].zadrzana = !kockice[i].zadrzana;
            return true;
        }
    }
    return false;
}

std::string UIKockice::tekstVrijednosti(UIKockicaVrijednost v)
{
    switch (v)
    {
    case UIKockicaVrijednost::One:   return "1";
    case UIKockicaVrijednost::Two:   return "2";
    case UIKockicaVrijednost::Three: return "3";
    default: return "";
    }
}

void UIKockice::nacrtajIkonu(sf::RenderWindow& prozor, const sf::FloatRect& r, UIKockicaVrijednost v) const
{
    if (!ikoneUcitan) return;

    const sf::Texture* tex = nullptr;
    if (v == UIKockicaVrijednost::Heart)  tex = &texHeart;
    if (v == UIKockicaVrijednost::Claw)   tex = &texClaw;
    if (v == UIKockicaVrijednost::Energy) tex = &texEnergy;
    if (!tex) return;

    sf::Sprite s(*tex);

    // skaliranje da stane u kockicu
    auto ts = tex->getSize();
    float target = r.size.x * 0.75f;

    float scaleX = target / (float)ts.x;
    float scaleY = target / (float)ts.y;
    float sc = std::min(scaleX, scaleY);

    s.setScale({ sc, sc });

    float w = (float)ts.x * sc;
    float h = (float)ts.y * sc;

    s.setPosition({
        r.position.x + (r.size.x - w) / 2.f,
        r.position.y + (r.size.y - h) / 2.f
        });

    prozor.draw(s);
}

void UIKockice::nacrtaj(sf::RenderWindow& prozor, const sf::Font& font) const
{
    // naslov iznad kockica
    sf::Text t(font,
        "Preostalo bacanja: " + std::to_string(preostalaBacanja),
        20
    );
    t.setFillColor(sf::Color::White);
    t.setPosition({ pocetak.x, pocetak.y - 32.f });
    prozor.draw(t);

    for (int i = 0; i < (int)kockice.size(); i++)
    {
        const auto& k = kockice[i];
        const auto& r = rects[i];

        // glow ako je zadrzana
        if (k.zadrzana)
        {
            sf::RectangleShape glow({ r.size.x + 10.f, r.size.y + 10.f });
            glow.setOrigin({ 5.f,5.f });
            glow.setPosition(r.position);
            glow.setFillColor(sf::Color::Transparent);
            glow.setOutlineThickness(4.f);
            glow.setOutlineColor(sf::Color(255, 200, 0));
            prozor.draw(glow);
        }

        // kutija kockice
        sf::RectangleShape box(r.size);
        box.setPosition(r.position);
        box.setFillColor(sf::Color(70, 70, 70));
        box.setOutlineThickness(2.f);
        box.setOutlineColor(sf::Color::White);
        prozor.draw(box);

        // PRAZNA KOCKICA 
        if (k.prazna)
            continue;

        // broj ili ikona
        if (k.vrijednost == UIKockicaVrijednost::One ||
            k.vrijednost == UIKockicaVrijednost::Two ||
            k.vrijednost == UIKockicaVrijednost::Three)
        {
            sf::Text v(font, tekstVrijednosti(k.vrijednost), 28);
            v.setFillColor(sf::Color::White);
            auto b = v.getLocalBounds();
            v.setOrigin({ b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f });
            v.setPosition({
                r.position.x + r.size.x / 2.f,
                r.position.y + r.size.y / 2.f
                });
            prozor.draw(v);
        }
        else
        {
            nacrtajIkonu(prozor, r, k.vrijednost);
        }
    }
}

std::vector<int> UIKockice::indeksiZaReroll() const
{
    std::vector<int> out;
    out.reserve((int)kockice.size());

    for (int i = 0; i < (int)kockice.size(); i++)
    {
        if (!kockice[i].zadrzana)
            out.push_back(i);
    }

    return out;
}

void UIKockice::ponistiZadrzavanje()
{
    for (auto& k : kockice)
        k.zadrzana = false;
}