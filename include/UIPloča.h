#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

// 
struct UIIgrac
{
    std::string ime = "Igrac";
    int lp = 10;
    int vp = 0;
    int ec = 0;

    bool ziv = true;
    bool uTokyu = false;

    sf::Color boja = sf::Color::White;

    const sf::Texture* ikona = nullptr;

    UIIgrac() = default; //eksplicitni defaultni konstruktor
};

class UIPloèa
{
public:
    UIPloèa() = default;

    // 1920x1080 
    void postaviEkran(const sf::Vector2u& velicina);

    void postaviNaslovDesnoGore(const std::string& tekst);
    void postaviInfoLijevoGore(const std::string& tekst);

    void postaviIgrace(const std::vector<UIIgrac>& igraci, int indeksTrenutnog);

    void nacrtaj(sf::RenderWindow& prozor, const sf::Font& font) const;

private:
    // stanje 
    sf::Vector2u velEkrana{ 1920u, 1080u };
    std::string tekstNaslov = "KING OF TOKYO";
    std::string tekstInfo = "";

    std::vector<UIIgrac> igraciUI;
    int indeksTrenutnogUI = 0;

    // layout
    float visinaHeader = 90.f;
    float visinaDonjeRezerve = 120.f; 

    sf::Vector2f centarTokyo{ 960.f, 540.f };
    float radijusTokyo = 230.f;

    float radijusIgrac = 120.f;
    float radijusPrsten = 360.f;

    std::vector<sf::Vector2f> pozicije; // po indeksu igraca

    
    void izracunajLayout();
    int indeksTokya() const;

    void nacrtajHeader(sf::RenderWindow& prozor, const sf::Font& font) const;
    void nacrtajTokyo(sf::RenderWindow& prozor, const sf::Font& font) const;
    void nacrtajIgrace(sf::RenderWindow& prozor, const sf::Font& font) const;

    static void centrirajTekst(sf::Text& t);
};
