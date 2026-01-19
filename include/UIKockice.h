#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

enum class UIKockicaVrijednost
{
    One,
    Two,
    Three,
    Heart,
    Energy,
    Claw
};

struct UIKockica
{
    UIKockicaVrijednost vrijednost = UIKockicaVrijednost::One;
    bool zadrzana = false; // KEEP
    bool prazna = false;
};

class UIKockice
{
public:
    UIKockice() = default;

    void postaviEkran(const sf::Vector2u& velicina);

    //  NE resetira zadrzana stanje (glow ostaje)
    void postaviKockice(const std::vector<UIKockica>& nove);

    void postaviPreostalaBacanja(int br);

    // Uèitaj ikonice iz assets (heart.png, claw.png, lightning.png)
    bool ucitajIkone(const std::string& folder = "assets");

    // klik (toggle keep)
    bool obradiKlik(const sf::Vector2f& misPozicija);

    void nacrtaj(sf::RenderWindow& prozor, const sf::Font& font) const;

    // reroll = sve koje NISU zadrzane
    std::vector<int> indeksiZaReroll() const;

    // nakon POTVRDI
    void ponistiZadrzavanje();

private:
    sf::Vector2u velEkrana{ 1920u,1080u };

    std::vector<UIKockica> kockice;
    int preostalaBacanja = 0;

    float velicinaKockice = 64.f;
    float razmak = 12.f;

    // Donji lijevi kut
    sf::Vector2f pocetak{ 40.f, 0.f };

    std::vector<sf::FloatRect> rects;

    // Ikonice
    bool ikoneUcitan = false;
    sf::Texture texHeart;
    sf::Texture texClaw;
    sf::Texture texEnergy;

    void izracunajLayout();
    static std::string tekstVrijednosti(UIKockicaVrijednost v);

    void nacrtajIkonu(sf::RenderWindow& prozor, const sf::FloatRect& r, UIKockicaVrijednost v) const;
};
