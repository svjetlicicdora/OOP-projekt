#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "Game.h"
#include "UIPloèa.h"
#include "UIKockice.h"
#include "UIGumb.h"
#include "UIPocetniEkran.h"
#include "UIPravila.h"
#include "UIDeckPanel.h"
#include "GameOverEkran.h"



class UIManager
{
public:
    UIManager();
    void pokreni();
    
private:
    enum class StanjeUI
    {
        PocetniEkran,
        Igra,
        Pravila,
        KupnjaKarte,
        GameOver
    };

    UIPravila pravila;

    // SFML
    sf::RenderWindow prozor;
    sf::Font font;

    // igra + stanje
    Game igra;
    bool potezZapoceo = false;

    // AI pauze
    bool aiUTijeku = false;
    sf::Clock aiSat;
    float aiPauzaSek = 1.0f;

    // UI komponente
    UIPloèa ploca;
    UIKockice uiKockice;
    UIGumb gumbBaci;
    UIGumb gumbPotvrdi;
    UIDeck uiDeck;

    // poèetni ekran
    UIPocetniEkran pocetni;
    StanjeUI stanje = StanjeUI::PocetniEkran;

    bool kupnjaPonudenaPotez = false; //flag za kupovinu kartica ako igraè ima dovoljno ec-a
    

    void inicijalizirajSFML();
    void ucitajFont();
    void inicijalizirajUI();


    // loop
    void obradiDogadaje();
    void obradiKlik(const sf::Vector2f& mis);
    void tickAI();
    void nacrtaj();

    // 
    void osvjeziHeader();
    void osvjeziGumbe();
    void osvjeziSveIzIgre();

    // mapiranja
    static UIKockicaVrijednost mapirajKockicu(DiceFace f);
    std::vector<UIKockica> napraviUIKockiceIzIgre() const;
    std::vector<UIIgrac> napraviUIIgrace() const;
    std::vector<sf::Texture> tekstureLikova;

    // 
    sf::FloatRect rectYieldIzlazim;
    sf::FloatRect rectYieldOstajem;

    void izracunajYieldLayout();
    void nacrtajYieldOverlay();
    void nacrtajJednostavanGumb(const sf::FloatRect& r, const std::string& tekst, bool aktivan);
    
    bool kupnjaKarteAktivna = false;

    sf::FloatRect rectKupiDa;
    sf::FloatRect rectKupiNe;

    void izracunajKupnjuLayout();
    void nacrtajKupnjuOverlay();
    GameOverEkran gameOver;
    std::unordered_map<std::string, const sf::Texture*> mapaIkona;




};
