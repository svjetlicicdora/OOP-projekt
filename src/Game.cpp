#include "Game.h"
#include "AIController.h"
#include "HumanController.h"
#include "Kartice10.h"
#include "Player.h"

#include <stdexcept>
#include <algorithm>
#include <unordered_map>

//je li ispravan indeks igraca
void Game::validatePlayerIndex(int idx) const
{
	if (idx < 0 || idx >= static_cast<int>(players.size())) {
		throw std::out_of_range("Neispravan indeks igrača");
	}
}

//je li ispravni indeksi kockica
void Game::validateDiceIndices(const std::vector<int>& indices) const
{
	for (int i : indices)
	{
		if(i<0 || i>=6)
			throw std::invalid_argument("Indeks kockice je izvan raspona(mora biti 0-5)");
	}
}

//broj živih igrača
int Game::countAlivePlayers() const
{
	int alive = 0;
	for (const auto& p : players) {
		if (p->isAlive()) {
			alive++;
		}
	}
	return alive;
}
//indeks zadnjeg živog igrača
int Game::findLastAlivePlayerIndex() const
{
	int last = -1;
	for(int i=0; i<static_cast<int>(players.size()); i++)
	{
		if (players[i]->isAlive()) {
			last = i;
		}
	}
	return last;
}

void Game::awardTokyoStartBonus() // igrač dobij + 2 VP ako započne potez u Tokiju
{
	if(tokyoIndex == currentPlayerIndex)
	{
		players[currentPlayerIndex]->addVP(2);
		lastMessage = "Bonus za početak poteza u Tokiju: +2 VP";
	}
}

//
const Player& Game::getPlayer(int idx) const
{
	validatePlayerIndex(idx);
	return *(players[idx]);
}

//postavlja igru u početno stanje
void Game::init(const std::vector<std::string>& playerNames)
{
	if(playerNames.size()<2||playerNames.size()>6)
		throw std::invalid_argument("Broj igrača mora biti između 2 i 6");
	players.clear();
	players.reserve(playerNames.size());
	controllers.clear();
	controllers.reserve(playerNames.size());

	for(const auto& name : playerNames)
	{
		players.push_back(std::make_unique<Player>(name));
		
		controllers.push_back(std::make_unique<AIController>());
	}

	currentPlayerIndex = 0;
	tokyoIndex = -1;
	rollsleft = 0;
	selectedForReroll.clear();


	gameOver = false;
	winnerIndex = -1;

	yieldDecisionRequired = false;
	pendingYieldTokyoIndex = -1;
	pendingYieldAttackerIndex = -1;
	lastIncomingDamageTokyo = 0;

	odbaceneKarte.clear();
	ZadnjaNaziv.clear();
	ZadnjaOpis.clear();
	deck = KarticeDeck(); // ponovno puni špil


	lastMessage = "Igra je inicijalizirana";
}

void Game::nextPlayer()
{
	if (players.empty())
		throw std::runtime_error("Igra nije inicijalizirana");

	int n = static_cast<int>(players.size());
	for (int korak = 0; korak < n; korak++)
	{
		currentPlayerIndex = (currentPlayerIndex + 1) % n;
		if (players[currentPlayerIndex]->isAlive())
			return;
	}

	// ako nitko nije živ → game over
	gameOver = true;
	winnerIndex = -1;
	lastMessage = "Nijedan igrač nije preživio. Igra je završena bez pobjednika.";
}
//početak poteza-početno stanje kod novog poteza
void Game::beginTurn()
{
	if (gameOver)
		throw std::runtime_error("Igra je završena. Nije moguće započeti novi potez");
	if (!players[currentPlayerIndex]->isAlive())
		nextPlayer();

	rollsleft = 3;
	selectedForReroll.clear();	

	yieldDecisionRequired = false;
	pendingYieldTokyoIndex = -1;
	pendingYieldAttackerIndex = -1;

	lastMessage = "Potez igrača " + players[currentPlayerIndex]->get_ime() + " je započeo";

	std::string before = lastMessage;
	awardTokyoStartBonus();
	if(tokyoIndex != currentPlayerIndex)
	{
		lastMessage = before; //ukloni poruku o bonusu ako nije u Tokiju
	}
}

//postavlja indekse kockica za reroll
void Game::setSelectedForReroll(const std::vector<int>& indices)
{
	validateDiceIndices(indices);
	selectedForReroll = indices;
	lastMessage = "Odabrane kockica za reroll su ažurirane";
}

//bacanje kockica
void Game::roll()
{
	if (gameOver)
		throw std::runtime_error("Igra je završena. Nije moguće baciti kockice");
	if(yieldDecisionRequired)
		throw std::runtime_error("Potrebna je odluka o izlasku iz Tokija prije ponovnog bacanja");
	if (rollsleft <= 0)
		throw std::runtime_error("Nema više bacanja. Potvrdite potez");

	if (rollsleft == 3) // prvo bacanje - sve kockice
	{
		diceSet.roll_all();
		lastMessage = "Prvo bacanje kockica je izvršeno";
	}
	else // ponovna bacanja - samo neoznačene kockice
	{
		diceSet.roll_selected(selectedForReroll);
		lastMessage = "Ponovno bacanje označenih kockica je izvršeno";
	}
	rollsleft--;
	//selectedForReroll.clear();
}

//potvrda poteza i razrješenje ishoda
void Game::confirmTurn()
{
	if (gameOver)
		throw std::runtime_error("Igra je završena. Nije moguće potvrditi potez.");
	if(yieldDecisionRequired)
		throw std::runtime_error("Potrebna je odluka o izlasku iz Tokija prije potvrde poteza");
	rollsleft = 0;
	resolveTurn();
	checkGameOver();

	if(!gameOver)
		if (!yieldDecisionRequired)
		{
			
			lastMessage = "Potez igrača " + players[currentPlayerIndex]->get_ime() + " je završen.";	

		}
}


//funkcija koja primjenjuje odluku o izlasku iz Tokija
void Game::applyYieldDecision(bool yield)
{
	if (!yieldDecisionRequired)
		throw std::runtime_error("Nije potrebna odluka o izlasku iz Tokija");

	if (yield)
	{
		// Igrač u Tokyju izlazi → napadač ulazi
		tokyoIndex = pendingYieldAttackerIndex;

		
		if (!players[tokyoIndex]->isAlive())
			tokyoIndex = -1;

		if (tokyoIndex != -1)
			players[tokyoIndex]->addVP(1);

		lastMessage =
			"Igrač " + players[pendingYieldTokyoIndex]->get_ime() +
			" je napustio Tokyo. Igrač " +
			(tokyoIndex != -1 ? players[tokyoIndex]->get_ime() : std::string("nitko")) +
			" ulazi u Tokyo i dobiva +1 VP.";
	}
	else
	{
		// Igrač ostaje u Tokyju
		lastMessage =
			"Igrač " + players[pendingYieldTokyoIndex]->get_ime() +
			" ostaje u Tokiju.";
	}

	yieldDecisionRequired = false;
	pendingYieldTokyoIndex = -1;
	pendingYieldAttackerIndex = -1;
}
//razrješenje poteza

void Game::resolveTurn()
{
	std::unordered_map<DiceFace, int> cnt;
	for (DiceFace f : diceSet.get_lica())
	{
		cnt[f]++;
	}

	Player& currentPlayer = *players[currentPlayerIndex];



	lastIncomingDamageTokyo = 0;
	//human igrač pošto je prvi mora ući u tokyo bez obzira na ishod kockica
	if (tokyoIndex == -1 && currentPlayerIndex == 0)
	{
		tokyoIndex = 0;
		currentPlayer.addVP(1);
		lastMessage = "Igrač " + currentPlayer.get_ime() + " ulazi u Tokyo i dobiva +1 VP.";
	}


	// VP iz brojeva (1,2,3)
	auto scoreNumber = [&](DiceFace face, int baseValue)
		{
			int k = cnt[face];
			if (k >= 3) {
				int vp = baseValue + (k - 3);
				currentPlayer.addVP(vp);
			}
		};

	scoreNumber(DiceFace::One, 1);
	scoreNumber(DiceFace::Two, 2);
	scoreNumber(DiceFace::Three, 3);

	// EC
	int ecGain = cnt[DiceFace::Lightning];
	if (ecGain > 0) {
		currentPlayer.addEC(ecGain);
	}

	// HEAL (samo ako igrač nije u Tokyju)
	int healGain = cnt[DiceFace::Heart];
	if (healGain > 0 && tokyoIndex != currentPlayerIndex) {
		currentPlayer.heal(healGain);
	}

	// ATTACK (Claw)
	int damage = cnt[DiceFace::Claw];
	if (damage > 0)
	{
		if (tokyoIndex == -1)
		{
			// Tokyo prazan: current ulazi i dobiva +1VP
			tokyoIndex = currentPlayerIndex;
			currentPlayer.addVP(1);
			lastMessage = "Igrač " + currentPlayer.get_ime() + " ulazi u Tokyo i dobiva +1 VP.";
		}
		else if (tokyoIndex == currentPlayerIndex)
		{
			// current je u Tokyju: napada sve izvan Tokyja
			for (int i = 0; i < static_cast<int>(players.size()); i++)
			{
				if (i == tokyoIndex) continue;
				if (!players[i]->isAlive()) continue;
				players[i]->takeDamage(damage);
			}
			lastMessage = "Igrač " + currentPlayer.get_ime() +
				" šamara sve ostale igrače izvan Tokija za " +
				std::to_string(damage) + " štete.";
		}
		else
		{
			// current je izvan Tokyja: napada igrača u Tokyju
			Player& tokyoPlayer = *players[tokyoIndex];
			lastIncomingDamageTokyo = damage;
			tokyoPlayer.takeDamage(damage);

			if (tokyoPlayer.isAlive())
			{
				yieldDecisionRequired = true;
				pendingYieldTokyoIndex = tokyoIndex;
				pendingYieldAttackerIndex = currentPlayerIndex;

				lastMessage = "Igrač " + currentPlayer.get_ime() +
					" šamara igrača " + tokyoPlayer.get_ime() +
					" unutar Tokija za " + std::to_string(damage) +
					" štete. Potrebna je odluka hoće li igrač u Tokiju izaći.";
			}
			else
			{
				tokyoIndex = currentPlayerIndex;
				currentPlayer.addVP(1);

				lastMessage = "Igrač " + currentPlayer.get_ime() +
					" šamara igrača " + tokyoPlayer.get_ime() +
					" unutar Tokija za " + std::to_string(damage) +
					" štete. Igrač u Tokiju je umro. Igrač " +
					currentPlayer.get_ime() + " ulazi u Tokyo i dobiva +1 VP.";
			}
		}
	}

	// Na kraju poteza očisti selekciju rerolla
	selectedForReroll.clear();
}

//provjera završetka igre
void Game::checkGameOver()
{
	//  Ako je human mrtav → igra odmah završava
	if (!players[0]->isAlive())
	{
		gameOver = true;
		winnerIndex = findLastAlivePlayerIndex(); 
		lastMessage = "Human igrač je mrtav. Igra je završena.";
		return;
	}

	// Ako netko ima 20+ VP
	for (int i = 0; i < static_cast<int>(players.size()); i++)
	{
		if (!players[i]->isAlive())
			continue;

		if (players[i]->get_vp() >= 20)
		{
			gameOver = true;
			winnerIndex = i;
			lastMessage = "Igrač " + players[i]->get_ime() + " je dosegao 20 VP i pobjeđuje!";
			return;
		}
	}

	// Ako je ostao samo jedan igrač živ
	if (countAlivePlayers() == 1)
	{
		gameOver = true;
		winnerIndex = findLastAlivePlayerIndex();
		lastMessage = "Igrač " + players[winnerIndex]->get_ime() + " je posljednji preživjeli i pobjeđuje!";
		return;
	}
}

//kupovina nasumicne powerkarte
void Game::kupiRandomKartu()
{
	if (gameOver)
		throw std::runtime_error("Igra je završena. Nije moguće kupiti kartu.");

	if (yieldDecisionRequired)
		throw std::runtime_error("Prvo morate donijeti odluku o izlasku iz Tokija.");

	if (deck.prazno())
		throw std::runtime_error("Nema više karata u špilu.");

	Player& currentPlayer = *players[currentPlayerIndex];

	// izvuci kartu
	std::unique_ptr<PowerKartice> karta = deck.izvuciRandom();
	int cost = karta->cijena();

	// naplata 
	currentPlayer.potrosiEC(cost);

	// primijeni efekt
	karta->primijeni(*this, currentPlayer);

	// spremi za UI
	ZadnjaNaziv = karta->naziv();
	ZadnjaOpis = karta->opis();

	lastMessage = "Kupljena karta: " + ZadnjaNaziv + " (" + ZadnjaOpis + ")";

	// discard
	odbaceneKarte.push_back(std::move(karta));

	// nakon karte provjeri je li igra završila (
	checkGameOver();
}

//odigraj potez trenutnog AI igrača
void Game::playCurrentAITurn()
{
	if (gameOver)
		throw std::runtime_error("Igra je završena.");

	if (controllers.size() != players.size())
		throw std::runtime_error("Kontroleri nisu inicijalizirani pravilno.");

	beginTurn();

	int index = currentPlayerIndex;
	IController* ctrl = controllers[index].get();
	const Player& me = *players[index];

	while (rollsleft > 0)
	{
		std::vector<DiceFace> faces = diceSet.get_lica();
		std::vector<int> reroll = ctrl->odaberiKockicu(*this, me, faces, rollsleft);

		setSelectedForReroll(reroll);
		roll();
	}

	confirmTurn();

	if (yieldDecisionRequired)
	{
		// ako je human u Tokyju AI ne odlučuje, UI rješava
		if (pendingYieldTokyoIndex == 0)
		{
			return; // potez završava, currentPlayer ostaje napadač
		}

		// AI je u Tokyju  AI u Tokyju odlučuje
		int tokyoIdx = pendingYieldTokyoIndex;
		IController* yieldCtrl = controllers[tokyoIdx].get();
		const Player& tokyoPlayer = *players[tokyoIdx];

		bool yield = yieldCtrl->decideYield(*this, tokyoPlayer, lastIncomingDamageTokyo);
		applyYieldDecision(yield);
	}

	nextPlayer();
}

//dodaj energiju trenutnom igraču
void Game::dodajEnergijuTrenutnom(int iznos)
{
	if (players.empty())
		throw std::runtime_error("Igra nije inicijalizirana.");

	players[currentPlayerIndex]->addEC(iznos);
}

void Game::reset()
{
	players.clear();
	currentPlayerIndex = 0;
	
}

