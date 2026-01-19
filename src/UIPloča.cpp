#include "UIPloèa.h"
#include <cmath>
#include <algorithm>

static constexpr float PI = 3.14159265358979323846f;

void UIPloèa::postaviEkran(const sf::Vector2u& velicina)
{
    velEkrana = velicina;
    izracunajLayout();
}

void UIPloèa::postaviNaslovDesnoGore(const std::string& tekst)
{
    tekstNaslov = tekst;
}

void UIPloèa::postaviInfoLijevoGore(const std::string& tekst)
{
    tekstInfo = tekst;
}

void UIPloèa::postaviIgrace(const std::vector<UIIgrac>& igraci, int indeksTrenutnog)
{
    igraciUI = igraci;
    indeksTrenutnogUI = std::clamp(indeksTrenutnog, 0, (int)igraciUI.size() - 1);
    izracunajLayout();
}

int UIPloèa::indeksTokya() const
{
    for (int i = 0; i < (int)igraciUI.size(); i++)
        if (igraciUI[i].uTokyu) return i;
    return -1;
}

void UIPloèa::izracunajLayout()
{
    const float W = (float)velEkrana.x;
    const float H = (float)velEkrana.y;

    const float gore = visinaHeader;
    const float dolje = H - visinaDonjeRezerve;

    const float radniW = W;
    const float radniH = std::max(100.f, dolje - gore);

    centarTokyo = { W * 0.5f, gore + radniH * 0.52f };

    // Tokyo krug — sada duplo manji
    float maxTokyo = std::min(radniW, radniH) * 0.14f;
    radijusTokyo = std::clamp(maxTokyo, 100.f, 140.f);

    // Prsten oko Tokya
    const float tekstMargina = 70.f;
    const float sigurnost = radijusIgrac + tekstMargina;

    float maxPrstenX = (radniW * 0.5f) - sigurnost;
    float maxPrstenY = (radniH * 0.5f) - sigurnost;

    float maxPrsten = std::min(maxPrstenX, maxPrstenY);
    maxPrsten += 80.f;
    float minPrsten = radijusTokyo + 180.f;

    if (maxPrsten < minPrsten)
        maxPrsten = minPrsten;

    float ciljani = radijusTokyo + 360.f; //udaljenist igraèa od tokya
    radijusPrsten = std::clamp(ciljani, minPrsten, maxPrsten);

    const int n = (int)igraciUI.size();
    pozicije.assign(n, centarTokyo);

    int tokyoIdx = indeksTokya();

    if (tokyoIdx >= 0 && tokyoIdx < n)
        pozicije[tokyoIdx] = centarTokyo;

    std::vector<int> prstenIndeksi;
    prstenIndeksi.reserve(n);

    for (int i = 0; i < n; i++)
        if (i != tokyoIdx) prstenIndeksi.push_back(i);

    int indeksZaGore = 0;
    if (tokyoIdx == 0)
        indeksZaGore = (prstenIndeksi.empty() ? 0 : prstenIndeksi[0]);

    float kutGore = -PI / 2.f;

    if (!prstenIndeksi.empty())
    {
        int pos = 0;
        for (int k = 0; k < (int)prstenIndeksi.size(); k++)
            if (prstenIndeksi[k] == indeksZaGore) { pos = k; break; }

        std::rotate(prstenIndeksi.begin(), prstenIndeksi.begin() + pos, prstenIndeksi.end());

        for (int k = 0; k < (int)prstenIndeksi.size(); k++)
        {
            float t = (float)k / (float)prstenIndeksi.size();
            float kut = kutGore + t * 2.f * PI;

            float x = centarTokyo.x + std::cos(kut) * radijusPrsten;
            float y = centarTokyo.y + std::sin(kut) * radijusPrsten;

            pozicije[prstenIndeksi[k]] = { x, y };
        }
            }
}



void UIPloèa::centrirajTekst(sf::Text& t)
{
    auto b = t.getLocalBounds();
    t.setOrigin({ b.position.x + b.size.x * 0.5f, b.position.y + b.size.y * 0.5f });
}

void UIPloèa::nacrtaj(sf::RenderWindow& prozor, const sf::Font& font) const
{
    nacrtajHeader(prozor, font);
    nacrtajTokyo(prozor, font);
    nacrtajIgrace(prozor, font);
}

void UIPloèa::nacrtajHeader(sf::RenderWindow& prozor, const sf::Font& font) const
{
    sf::Text info(font, tekstInfo, 22);
    info.setFillColor(sf::Color::White);
    info.setPosition({ 20.f, 20.f });
    prozor.draw(info);

    sf::Text naslov(font, tekstNaslov, 28);
    naslov.setFillColor(sf::Color::White);
    auto b = naslov.getLocalBounds();
    naslov.setOrigin({ b.position.x + b.size.x, 0.f });
    naslov.setPosition({ (float)velEkrana.x - 20.f, 18.f });
    prozor.draw(naslov);
}

void UIPloèa::nacrtajTokyo(sf::RenderWindow& prozor, const sf::Font& font) const
{
    sf::CircleShape tokyo(radijusTokyo);
    tokyo.setOrigin({ radijusTokyo, radijusTokyo });
    tokyo.setPosition(centarTokyo);
    tokyo.setFillColor(sf::Color(60, 60, 60));
    tokyo.setOutlineThickness(6.f);
    tokyo.setOutlineColor(sf::Color::White);
    prozor.draw(tokyo);

    sf::Text t(font, "TOKYO", 30);
    t.setFillColor(sf::Color::White);
    centrirajTekst(t);
    t.setPosition(centarTokyo);
    prozor.draw(t);
}

void UIPloèa::nacrtajIgrace(sf::RenderWindow& prozor, const sf::Font& font) const
{
    const int n = (int)igraciUI.size();
    if ((int)pozicije.size() != n) return;

    for (int i = 0; i < n; i++)
    {
        const auto& p = igraciUI[i];
        sf::Vector2f pos = pozicije[i];

        // Glow
        if (i == indeksTrenutnogUI)
        {
            sf::CircleShape glow(radijusIgrac + 10.f);
            glow.setOrigin(sf::Vector2f(radijusIgrac + 10.f, radijusIgrac + 10.f));
            glow.setPosition(pos);
            glow.setFillColor(sf::Color::Transparent);
            glow.setOutlineThickness(6.f);
            glow.setOutlineColor(sf::Color(255, 200, 0));
            prozor.draw(glow);
        }

        // Bijeli krug
        sf::CircleShape poz(radijusIgrac);
        poz.setOrigin(sf::Vector2f(radijusIgrac, radijusIgrac));
        poz.setPosition(pos);
        poz.setFillColor(sf::Color::White);
        poz.setOutlineThickness(3.f);
        poz.setOutlineColor(sf::Color::Black);
        prozor.draw(poz);

        // Ikona
        if (p.ikona != nullptr)
        {
            float w = (float)p.ikona->getSize().x;
            float h = (float)p.ikona->getSize().y;

            float target = radijusIgrac * 1.20f;
            float scale = target / std::max(w, h);

            sf::RectangleShape r;
            r.setSize(sf::Vector2f(w, h));
            r.setOrigin(sf::Vector2f(w / 2.f, h / 2.f));
            r.setScale(sf::Vector2f(scale, scale));
            r.setPosition(pos);
            r.setTexture(p.ikona);

            prozor.draw(r);
            // Ako je igrac mrtav —  crveni X preko ikone
            if (!p.ziv)
            {
                float w = radijusIgrac * 1.6f;   
                float thickness = 6.f;          

                // Prva dijagonala
                sf::RectangleShape lin1;
                lin1.setSize({ w, thickness });
                lin1.setFillColor(sf::Color::Red);
                lin1.setOrigin({ w / 2.f, thickness / 2.f });
                lin1.setPosition(pos);
                lin1.setRotation(sf::degrees(45.f));


                // Druga dijagonala
                sf::RectangleShape lin2;
                lin2.setSize({ w, thickness });
                lin2.setFillColor(sf::Color::Red);
                lin2.setOrigin({ w / 2.f, thickness / 2.f });
                lin2.setPosition(pos);
                lin2.setRotation(sf::degrees(-45.f));


                prozor.draw(lin1);
                prozor.draw(lin2);
            }
        }
        else
        {
            sf::CircleShape c(radijusIgrac);
            c.setOrigin(sf::Vector2f(radijusIgrac, radijusIgrac));
            c.setPosition(pos);

            sf::Color boja = p.boja;
            if (!p.ziv) boja = sf::Color(190, 190, 190);

            c.setFillColor(boja);
            c.setOutlineThickness(3.f);
            c.setOutlineColor(sf::Color::White);
            prozor.draw(c);
        }

       
      
        float tekstOffset = radijusIgrac + 24.f;

        // Ime
        std::string ime = p.ziv ? p.ime : "DEAD " + p.ime;

        sf::Text tIme(font, ime, 16);
        tIme.setFillColor(sf::Color::White);
        centrirajTekst(tIme);
        tIme.setPosition({ pos.x, pos.y - tekstOffset });
        prozor.draw(tIme);

        // Status
        std::string st = "LP: " + std::to_string(p.lp) +
            "  VP: " + std::to_string(p.vp) +
            "  EC: " + std::to_string(p.ec);

        sf::Text tSt(font, st, 24);
        tSt.setFillColor(sf::Color(220, 220, 220));
        centrirajTekst(tSt);
        tSt.setPosition({ pos.x, pos.y + tekstOffset });
        prozor.draw(tSt);
    }
}