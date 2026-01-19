#include "UIManager.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <fstream>
#include <direct.h>   
#include "DiceFace.h"


UIManager::UIManager()
{
    inicijalizirajSFML();
    ucitajFont();
    inicijalizirajUI();

   
    // IKONE ZA KOCKICE
    uiKockice.ucitajIkone("assets");


    izracunajYieldLayout();
    osvjeziSveIzIgre();
    izracunajKupnjuLayout();

}

void UIManager::inicijalizirajSFML()
{
    prozor = sf::RenderWindow(
        sf::VideoMode({ 1920u, 1080u }),
        "King of Tokyo - SFML",
        sf::Style::Titlebar | sf::Style::Close
    );
    prozor.setFramerateLimit(60);

    prozor.setView(sf::View(sf::FloatRect(
        { 0.f, 0.f },
        { (float)prozor.getSize().x, (float)prozor.getSize().y }
    )));
    pocetni.postaviEkran(prozor.getSize());

}

void UIManager::ucitajFont()
{
    if (!font.openFromFile("assets/DejaVuSans.ttf"))
        throw std::runtime_error("Ne mogu ucitati font assets/DejaVuSans.ttf");
}

void UIManager::inicijalizirajUI()
{
    

    pocetni.postaviEkran(prozor.getSize());
    ploca.postaviEkran(prozor.getSize());
    ploca.postaviNaslovDesnoGore("KING OF TOKYO");

     uiKockice.postaviEkran(prozor.getSize());

    gumbBaci.postaviEkran(prozor.getSize());
    gumbBaci.postaviTekst("BACI");
    gumbBaci.postaviDonjeDesno(1);

    gumbPotvrdi.postaviEkran(prozor.getSize());
    gumbPotvrdi.postaviTekst("POTVRDI");
    gumbPotvrdi.postaviDonjeDesno(0);


    uiDeck.postaviEkran(prozor.getSize());
    uiDeck.prikaziDeck();
    potezZapoceo = false;
    aiUTijeku = false;
    tekstureLikova.clear();

    std::vector<std::string> datoteke = {
        "Alenoid.png", "CyberKitty.png", "TheKing.png", "Gigazaur.png", "SpacePenguin.png", "MekaDragon.png"
    };

    for (auto& f : datoteke)
    {
        sf::Texture t;
        t.loadFromFile("assets/" + f);
        tekstureLikova.push_back(std::move(t));
    }

    mapaIkona.clear();
    mapaIkona["Alenoid"] = &tekstureLikova[0];
    mapaIkona["Cyber Kitty"] = &tekstureLikova[1];
    mapaIkona["The King"] = &tekstureLikova[2];
    mapaIkona["Gigazaur"] = &tekstureLikova[3];
    mapaIkona["Space Penguin"] = &tekstureLikova[4];
    mapaIkona["Meka Dragon"] = &tekstureLikova[5];


    gameOver.postaviEkran(prozor.getSize());
    gameOver.reset();
}

void UIManager::pokreni()
{
    while (prozor.isOpen())
    {
        obradiDogadaje();

        if (stanje==StanjeUI::Igra)
            tickAI();

        nacrtaj();
    }
}

void UIManager::obradiDogadaje()
{
    while (auto ev = prozor.pollEvent())
    {
        const sf::Event& e = *ev;

        // Zatvaranje prozora
        if (e.is<sf::Event::Closed>())
        {
            prozor.close();
            return;
        }

        // ESC → zatvori
        if (const auto* key = e.getIf<sf::Event::KeyPressed>())
        {
            if (key->code == sf::Keyboard::Key::Escape)
            {
                prozor.close();
                return;
            }
        }

      

      
        //  KLIK MIŠEM
       
        if (const auto* m = e.getIf<sf::Event::MouseButtonPressed>())
        {
            if (m->button == sf::Mouse::Button::Left)
            {
                sf::Vector2f mis = prozor.mapPixelToCoords(m->position);
                obradiKlik(mis);
            }
        }
    }
}

void UIManager::obradiKlik(const sf::Vector2f& mis)
{
    //  POČETNI EKRAN
    
    if (stanje == StanjeUI::PocetniEkran)
    {
        pocetni.obradiKlik(mis);

        if (pocetni.jeKliknutZapocni(mis) && pocetni.jeSpreman())
        {
            int broj = pocetni.dohvatiBrojIgraca();
            std::string ime = pocetni.dohvatiImeHumana();

// lista svih likova
            std::vector<std::string> sviLikovi = {
                "Cyber Kitty",
                "Gigazaur",
                "Alenoid",
                "Space Penguin",
                "The King",
                "Meka Dragon"
            };

            // makni humanov odabrani lik
            sviLikovi.erase(
                std::remove(sviLikovi.begin(), sviLikovi.end(), ime),
                sviLikovi.end()
            );

            // pripremi listu imena za igru
            std::vector<std::string> imena;
            imena.reserve(broj);

            // prvo dodaj human igrača
            imena.push_back(ime);

            // dodaj AI igrače s preostalim imenima
            for (int i = 0; i < broj - 1; i++)
            {
                imena.push_back(sviLikovi[i]);
            }

            igra.init(imena);

            igra.beginTurn();

            potezZapoceo = true;
            kupnjaPonudenaPotez = false;
            aiUTijeku = false;

            uiKockice.ponistiZadrzavanje();
            stanje = StanjeUI::Igra;

            osvjeziSveIzIgre();

            return;
        }

        if (pocetni.jeKliknutPravila(mis))
        {
            try
            {
                pravila.ucitajDatoteku();
                stanje = StanjeUI::Pravila;
            }
            catch (const std::exception& ex)
            {
                std::cout << "Greska pri ucitavanju pravila: " << ex.what() << "\n";
            }
            return;
        }

        return;
    }
//  GAME OVER
    if (stanje == StanjeUI::GameOver)
    {
        if (gameOver.obradiKlik(mis))
        {
            if (gameOver.jeKliknutaNovaIgra())
            {
                // RESET CIJELE IGRE
                igra.reset();          
                inicijalizirajUI();       // vrati se na početni ekran
                stanje = StanjeUI::PocetniEkran;
            }

            if (gameOver.jeKliknutIzlaz())
            {
                prozor.close();
            }
        }

        return; 
    }

    
    //  PRAVILA
   
    if (stanje == StanjeUI::Pravila)
    {
        if (pravila.obradiKlik(mis))
            stanje = StanjeUI::PocetniEkran;
        return;
    }

    //  KUPNJA KARTE (OVERLAY)
   
    if (stanje == StanjeUI::KupnjaKarte)
    {
        if (rectKupiDa.contains(mis))
        {
            try
            {
                igra.kupiRandomKartu();
                uiDeck.prikaziKartu(igra.getZadnjaNaziv(), igra.getZadnjaOpis());
            }
            catch (const std::exception& ex)
            {
                std::cout << "Greska pri kupnji karte: " << ex.what() << "\n";
            }

            stanje = StanjeUI::Igra;
            osvjeziSveIzIgre();
            return;
        }

        if (rectKupiNe.contains(mis))
        {
            stanje = StanjeUI::Igra;
            return;
        }

        return; // blokira sve ostalo dok je overlay aktivan
    }

    
    //  IGRA
    
    if (stanje != StanjeUI::Igra) return;
    if (igra.isGameOver())
    {
        stanje = StanjeUI::GameOver;
        gameOver.reset();
        return;
    }
    if (aiUTijeku) return;


    //  AUTOMATSKA PONUDA KUPNJE
  
    if (!kupnjaPonudenaPotez &&
        igra.getCurrentPlayerIndex() == 0 &&
        igra.getRollsLeft() == 3 &&            // početak poteza
        igra.getPlayer(0).get_ec() >= 3)
    {
        stanje = StanjeUI::KupnjaKarte;
        kupnjaPonudenaPotez = true;
        izracunajKupnjuLayout();
        return;
    }

    //  YIELD OVERLAY
  
    if (igra.isYieldDecisionRequired())
    {
        if (igra.getPendingYieldTokyoIndex() == 0)
        {
            if (rectYieldIzlazim.contains(mis))
            {
                igra.applyYieldDecision(true);
                igra.nextPlayer();
                osvjeziSveIzIgre();   

                kupnjaPonudenaPotez = false;

                // AI ili human
                if (igra.getCurrentPlayerIndex() != 0)
                {
                    aiUTijeku = true;
                    aiSat.restart();
                }
                else
                {
                    igra.beginTurn();
                    potezZapoceo = true;
                    uiKockice.ponistiZadrzavanje();
                }

                osvjeziSveIzIgre();
                return;
            }

            if (rectYieldOstajem.contains(mis))
            {
                igra.applyYieldDecision(false);
                igra.nextPlayer();
                osvjeziSveIzIgre();   

                kupnjaPonudenaPotez = false;

                //  AI ili human?
                if (igra.getCurrentPlayerIndex() != 0)
                {
                    aiUTijeku = true;
                    aiSat.restart();
                }
                else
                {
                    igra.beginTurn();
                    potezZapoceo = true;
                    uiKockice.ponistiZadrzavanje();
                }

                osvjeziSveIzIgre();
                return;
            }

            return;
        }

        // AI u Tokyju
        igra.applyYieldDecision(false);
        igra.nextPlayer();
        kupnjaPonudenaPotez = false;

        //  AI ili human?
        if (igra.getCurrentPlayerIndex() != 0)
        {
            aiUTijeku = true;
            aiSat.restart();
        }
        else
        {
            igra.beginTurn();
            potezZapoceo = true;
            uiKockice.ponistiZadrzavanje();
        }

        osvjeziSveIzIgre();
        return;
    }

    
    //  KLIK NA KOCKICE
  
    uiKockice.obradiKlik(mis);

    
    //  BACI
    
    if (gumbBaci.jeKliknut(mis))
    {

        try
        {
            if (!potezZapoceo)
            {
                igra.beginTurn();
                potezZapoceo = true;
                kupnjaPonudenaPotez = false;
                uiKockice.ponistiZadrzavanje();
                uiKockice.postaviKockice({});
                osvjeziHeader();      
                osvjeziGumbe();
                return;

            }

            auto reroll = uiKockice.indeksiZaReroll();
            igra.setSelectedForReroll(reroll);
            igra.roll();

            osvjeziSveIzIgre();
        }
        catch (const std::exception& ex)
        {
            std::cout << "Greska (BACI): " << ex.what() << "\n";
        }
        return;
    }
  
    //  POTVRDI

    if (gumbPotvrdi.jeKliknut(mis))
    {
        try
        {
            // Završetak bacanja i primjena efekata
            igra.confirmTurn();
            if (igra.isGameOver())
            {
                bool humanWon = igra.getPlayer(0).isAlive() && igra.countAlivePlayers() == 1;

                if (humanWon)
                    gameOver.postaviPoruku("POBJEDA!");
                else
                    gameOver.postaviPoruku("GAME OVER");

                stanje = StanjeUI::GameOver;
                gameOver.reset();
                return;
            }
            uiKockice.ponistiZadrzavanje();

            //  Ako je potrebna yield odluka → STOP
            if (igra.isYieldDecisionRequired())
            {
                return;
            }

            //  Yield NIJE potreban → prelazio na sljedećeg igrača
            igra.nextPlayer();

            
            kupnjaPonudenaPotez = false;

            //  Ako je AI na redu  pokreni AI
            if (igra.getCurrentPlayerIndex() != 0)
            {
                aiUTijeku = true;
                potezZapoceo = true;
                aiSat.restart();
                stanje = StanjeUI::Igra;   
            }
            else
            {
                // Human je na redu -započinje novi potez
                igra.beginTurn();
                potezZapoceo = true;
                kupnjaPonudenaPotez = false;
                uiKockice.ponistiZadrzavanje();
                uiKockice.postaviKockice({});
            }
            osvjeziSveIzIgre();
        }
        catch (const std::exception& ex)
        {
            std::cout << "Greska (POTVRDI): " << ex.what() << "\n";
        }

        return;
    }
}

void UIManager::tickAI()
{
    if (!aiUTijeku)
        return;

    if (igra.isGameOver())
    {
        aiUTijeku = false;
        stanje = StanjeUI::GameOver;
        gameOver.reset();
        return;
    }

    if (aiSat.getElapsedTime().asSeconds() < aiPauzaSek)
    {
        osvjeziHeader();
        return;
    }

    try
    {
        // AI odigra svoj potez
        igra.playCurrentAITurn();

        std::cout << "DEBUG: yield=" << igra.isYieldDecisionRequired()
            << " pending=" << igra.getPendingYieldTokyoIndex()
            << " current=" << igra.getCurrentPlayerIndex()
            << " alive=" << igra.getPlayer(igra.getCurrentPlayerIndex()).isAlive()
            << "\n";

        //  ako je AI umro tijekom poteza, UI se mora odmah osvježiti
        if (!igra.getPlayer(igra.getCurrentPlayerIndex()).isAlive())
        {
            osvjeziSveIzIgre();
        }

        // Ako human u Tokyju mora odlučiti - AI staje
        if (igra.isYieldDecisionRequired() && igra.getPendingYieldTokyoIndex() == 0)
        {
            aiUTijeku = false;
            aiSat.restart();
            osvjeziSveIzIgre();
            return;
        }

        // Ako je sljedeći igrač AI - nastavi AI loop
        if (igra.getCurrentPlayerIndex() != 0)
        {
            aiUTijeku = true;
            aiSat.restart();
            osvjeziSveIzIgre();
            return;
        }

        // Ako je human na redu -pripremi njegov potez
        aiUTijeku = false;
        igra.beginTurn();
        potezZapoceo = true;
        uiKockice.ponistiZadrzavanje();

        uiKockice.postaviKockice({}); // reset kockica
        osvjeziHeader();
        osvjeziGumbe();

        osvjeziSveIzIgre();

        aiSat.restart();
    }
    catch (const std::exception& ex)
    {
        std::cout << "Greska (AI): " << ex.what() << "\n";
        aiUTijeku = false;
    }
}
void UIManager::nacrtaj()
{
    
    prozor.clear(sf::Color::Black);

    
    //  POČETNI EKRAN
   
    if (stanje==StanjeUI::PocetniEkran)
    {
        pocetni.nacrtaj(prozor, font);
        prozor.display();
        return;
    }

    if (stanje == StanjeUI::GameOver)
    {
        gameOver.nacrtaj(prozor, font);
        prozor.display();
        return;
    }

    
    //  EKRAN IGRE
  
    if (stanje==StanjeUI::Igra)
    {
        ploca.nacrtaj(prozor, font);
        uiKockice.nacrtaj(prozor, font);
        gumbBaci.nacrtaj(prozor, font);
        gumbPotvrdi.nacrtaj(prozor, font);
        uiDeck.nacrtaj(prozor, font);

        sf::Text msg(font, "Poruka: " + igra.getLastMessage(), 20);
        msg.setFillColor(sf::Color::White);
        msg.setPosition({ 40.f, 1080.f - 220.f });
        prozor.draw(msg);

        if (igra.isYieldDecisionRequired() && igra.getPendingYieldTokyoIndex() == 0)
            nacrtajYieldOverlay();

        prozor.display();
        return;
    }
    if (stanje == StanjeUI::Pravila)
    {
        pravila.nacrtaj(prozor, font);
        prozor.display();
        return;
    }
    if (kupnjaKarteAktivna)
    {
        nacrtajKupnjuOverlay();
    }
    if (stanje == StanjeUI::KupnjaKarte)
    {
        // nacrtaj normalnu igru ispod
        ploca.nacrtaj(prozor, font);
        uiKockice.nacrtaj(prozor, font);
        gumbBaci.nacrtaj(prozor, font);
        gumbPotvrdi.nacrtaj(prozor, font);
        uiDeck.nacrtaj(prozor, font);

        // overlay iznad svega
        nacrtajKupnjuOverlay();

        prozor.display();
        return;

        //EKRAN ZA GAMEOVE
        if (stanje == StanjeUI::GameOver)
        {
            prozor.clear(sf::Color::Black);
            gameOver.nacrtaj(prozor, font);
            prozor.display();
            return;
        }
    }

    prozor.display();
}
void UIManager::osvjeziHeader()
{
    int curr = igra.getCurrentPlayerIndex();
    std::string imeTrenutnog = igra.getPlayer(curr).get_ime();

    std::string uTokyu = "nitko";
    int tokyo = igra.getTokyoIndex();
    if (tokyo >= 0 && tokyo < igra.getPlayerCount())
        uTokyu = igra.getPlayer(tokyo).get_ime();

    std::string dodatno = "";
    if (aiUTijeku && curr != 0)
        dodatno = "  |  " + imeTrenutnog + " razmislja...";

    ploca.postaviInfoLijevoGore(
        "Trenutni igrac: " + imeTrenutnog +
        "  |  U Tokyu: " + uTokyu +
        dodatno
    );
}

void UIManager::osvjeziGumbe()
{
    bool omoguci = (stanje==StanjeUI::Igra) &&
        !igra.isGameOver() &&
        !igra.isYieldDecisionRequired() &&
        !aiUTijeku;

    gumbBaci.postaviOmogucen(omoguci);
    gumbPotvrdi.postaviOmogucen(omoguci);
}

void UIManager::osvjeziSveIzIgre()
{
 
    if (stanje!= StanjeUI::Igra)
        return;

    auto uiIgraci = napraviUIIgrace();
    ploca.postaviIgrace(uiIgraci, igra.getCurrentPlayerIndex());

    osvjeziHeader();



    uiKockice.postaviKockice(napraviUIKockiceIzIgre());
    uiKockice.postaviPreostalaBacanja(igra.getRollsLeft());

    osvjeziGumbe();
}


UIKockicaVrijednost UIManager::mapirajKockicu(DiceFace f)
{
    switch (f)
    {
   
    case DiceFace::One:       return UIKockicaVrijednost::One;
    case DiceFace::Two:       return UIKockicaVrijednost::Two;
    case DiceFace::Three:     return UIKockicaVrijednost::Three;
    case DiceFace::Heart:     return UIKockicaVrijednost::Heart;
    case DiceFace::Lightning: return UIKockicaVrijednost::Energy;
    case DiceFace::Claw:      return UIKockicaVrijednost::Claw;
    default:                  return UIKockicaVrijednost::One;
    }
}

std::vector<UIKockica> UIManager::napraviUIKockiceIzIgre() const
{
    std::vector<DiceFace> faces = igra.getDiceFaces();

    std::vector<UIKockica> out;
    out.reserve(6);

    
    if (faces.empty())
    {
        for (int i = 0; i < 6; i++)
        {
           UIKockica k;
            k.vrijednost = UIKockicaVrijednost::One; 
            k.prazna = true;// prazan kvadrati
            out.push_back(k);
        }
        return out;
    }

    
    while ((int)faces.size() < 6)
        faces.push_back(DiceFace::One);

    
    for (int i = 0; i < 6; i++)
    {
        UIKockica k;
        k.vrijednost = mapirajKockicu(faces[i]);
       

        out.push_back(k);
    }

    return out;
}

std::vector<UIIgrac> UIManager::napraviUIIgrace() const
{
    std::vector<UIIgrac> ui;
    int n = igra.getPlayerCount();
    ui.reserve(n);

    static std::vector<sf::Color> boje = {
        sf::Color(60,120,180),
        sf::Color(220,80,80),
        sf::Color(80,200,120),
        sf::Color(220,180,80),
        sf::Color(160,90,220),
        sf::Color(80,200,210)
    };

    int tokyoIdx = igra.getTokyoIndex();

    for (int i = 0; i < n; i++)
    {
        const Player& p = igra.getPlayer(i);

        UIIgrac x;
        x.ime = p.get_ime();
        x.lp = p.get_lp();
        x.vp = p.get_vp();
        x.ec = p.get_ec();

        x.ziv = p.isAlive();
        x.uTokyu = (i == tokyoIdx);

        x.boja = boje[i % (int)boje.size()];
        if (!x.ziv)
            x.boja = sf::Color(170, 170, 170);

        // dodjela ikone ako postoji
        auto it = mapaIkona.find(x.ime);
        if (it != mapaIkona.end())
            x.ikona = it->second;
        else
            x.ikona = nullptr; 
            
        ui.push_back(x);
    }

    return ui;
}
void UIManager::izracunajYieldLayout()
{
    float W = (float)prozor.getSize().x;
    float H = (float)prozor.getSize().y;

    float gW = 220.f;
    float gH = 60.f;
    float razmak = 30.f;

    float cx = W / 2.f;
    float y = H / 2.f + 40.f;

    rectYieldIzlazim = sf::FloatRect(
        { cx - gW - razmak / 2.f, y },
        { gW, gH }
    );

    rectYieldOstajem = sf::FloatRect(
        { cx + razmak / 2.f, y },
        { gW, gH }
    );
}

void UIManager::nacrtajJednostavanGumb(const sf::FloatRect& r, const std::string& tekst, bool aktivan)
{
    sf::RectangleShape box(r.size);
    box.setPosition(r.position);
    box.setFillColor(aktivan ? sf::Color(45, 45, 45) : sf::Color(25, 25, 25));
    box.setOutlineThickness(3.f);
    box.setOutlineColor(sf::Color::White);
    prozor.draw(box);

    sf::Text t(font, tekst, 22);
    t.setFillColor(sf::Color::White);

    auto b = t.getLocalBounds();
    t.setOrigin({ b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f });
    t.setPosition({ r.position.x + r.size.x / 2.f, r.position.y + r.size.y / 2.f });
    prozor.draw(t);
}

void UIManager::nacrtajYieldOverlay()
{
    sf::RectangleShape dim(sf::Vector2f((float)prozor.getSize().x, (float)prozor.getSize().y));
    dim.setPosition({ 0.f, 0.f });
    dim.setFillColor(sf::Color(0, 0, 0, 170));
    prozor.draw(dim);

    std::string poruka = "Zelis li napustiti Tokyo?";
    int tokyoIdx = igra.getPendingYieldTokyoIndex();
    if (tokyoIdx >= 0 && tokyoIdx < igra.getPlayerCount())
    {
        poruka = "Zelis li napustiti Tokyo?\n"
            "Igrac u Tokyu: " + igra.getPlayer(tokyoIdx).get_ime() +
            "  |  Dolazeca steta: " + std::to_string(igra.getLastIncomingDamageTokyo());
    }

    sf::Text t(font, poruka, 34);
    t.setFillColor(sf::Color::White);

    auto b = t.getLocalBounds();
    t.setOrigin({ b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f });
    t.setPosition({ (float)prozor.getSize().x / 2.f, (float)prozor.getSize().y / 2.f - 60.f });
    prozor.draw(t);

    nacrtajJednostavanGumb(rectYieldIzlazim, "IZLAZIM", true);
    nacrtajJednostavanGumb(rectYieldOstajem, "OSTAJEM", true);
}

void UIManager::izracunajKupnjuLayout()
{
    float W = (float)prozor.getSize().x;
    float H = (float)prozor.getSize().y;

    float gW = 220.f;
    float gH = 60.f;
    float razmak = 30.f;

    float cx = W / 2.f;
    float y = H / 2.f + 80.f;

    rectKupiDa = sf::FloatRect(
        { cx - gW - razmak / 2.f, y },
        { gW, gH }
    );

    rectKupiNe = sf::FloatRect(
        { cx + razmak / 2.f, y },
        { gW, gH }
    );
}

void UIManager::nacrtajKupnjuOverlay()
{
    sf::RectangleShape dim(
        sf::Vector2f((float)prozor.getSize().x, (float)prozor.getSize().y)
    );
    dim.setFillColor(sf::Color(0, 0, 0, 170));
    prozor.draw(dim);

    sf::Text t(font,
        "Zelis li kupiti Power-up kartu za 3 energije?",
        32
    );
    t.setFillColor(sf::Color::White);

    auto b = t.getLocalBounds();
    t.setOrigin({ b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f });
    t.setPosition({
        (float)prozor.getSize().x / 2.f,
        (float)prozor.getSize().y / 2.f - 40.f
        });

    prozor.draw(t);

    nacrtajJednostavanGumb(rectKupiDa, "KUPI", true);
    nacrtajJednostavanGumb(rectKupiNe, "PRESKOCI", true);
}




