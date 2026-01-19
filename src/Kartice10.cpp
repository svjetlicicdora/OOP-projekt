#include "Kartice10.h"
#include "Game.h"
#include "Player.h"

void KartaSlava::primijeni(Game&, Player& igrac) { igrac.addVP(2); }
void KartaTrofej::primijeni(Game&, Player& igrac) { igrac.addVP(1); }

void KartaGenerator::primijeni(Game&, Player& igrac) { igrac.addEC(3); }
void KartaBaterija::primijeni(Game&, Player& igrac) { igrac.addEC(2); }

void KartaPrvaPomoc::primijeni(Game& game, Player& igrac)
{
    // heal samo ako igraè nije u Tokyju
    if (game.getTokyoIndex() != game.getCurrentPlayerIndex())
        igrac.heal(2);
}

void KartaRegeneracija::primijeni(Game& game, Player& igrac)
{
    if (game.getTokyoIndex() != game.getCurrentPlayerIndex())
        igrac.heal(1);
}

void KartaJackpot::primijeni(Game&, Player& igrac)
{
    igrac.addVP(1);
    igrac.addEC(2);
}

void KartaNagrada::primijeni(Game&, Player& igrac) { igrac.addVP(3); }
void KartaSuperPunjenje::primijeni(Game&, Player& igrac) { igrac.addEC(5); }

void KartaMalaNagrada::primijeni(Game&, Player& igrac)
{
    igrac.addVP(1);
    igrac.addEC(1);
}