#pragma once

#include <vector>
#include <string>
#include <memory>


#include "Player.h"
#include "DiceSet.h"
#include "DiceFace.h"
#include "IController.h"
#include "KarticeDeck.h"
#include "PowerKartice.h"

class Game
{
private:
	std::vector<std::unique_ptr<Player>>players;
	std::vector<std::unique_ptr<IController>>controllers; //kontroleri igraèa (human ili AI)
	DiceSet diceSet;

	int currentPlayerIndex = 0;
	int tokyoIndex = -1;

	//koliko bacanja je igraèu ostalo na njegovom potezu
	int rollsleft = 0;
	std::vector<int>selectedForReroll; // indeksi kockica koje je igraè oznaèio za ponovni bacanje

	bool gameOver = false;
	int winnerIndex = -1;
	int lastIncomingDamageTokyo = 0;

	std::string lastMessage; //za UI prikaz posljednje poruke o dogaðajima u igri

	bool yieldDecisionRequired = false; //je li igraèu potreban unos da odluèi hoæe li izaæi iz Tokija
	int pendingYieldTokyoIndex = -1; //odluèuje igraè u Tokiju hoæe li izaæi
	int pendingYieldAttackerIndex = -1; //tko ulazi u Tokiju ako igraè izaðe

	KarticeDeck deck;
	std::vector<std::unique_ptr<PowerKartice>> odbaceneKarte;
	std::string ZadnjaNaziv;
	std::string ZadnjaOpis;

	void validatePlayerIndex(int idx) const;
	void validateDiceIndices(const std::vector<int>& indices) const;

	
	int findLastAlivePlayerIndex() const;

	//funkcija ako igraè zapoène svoj potez u Tokiju
	void awardTokyoStartBonus();
	void resolveTurn();
	void checkGameOver();
public:
	Game() = default;
	int countAlivePlayers() const;
	//inicijalizacija igraèa - human æe biti playerNames[0}, a ostali æe biti AI

	void init(const std::vector<std::string>& playerNames);

	//stanje za UI

	int getCurrentPlayerIndex() const { return currentPlayerIndex; }
	int getTokyoIndex() const { return tokyoIndex; }
	int getRollsLeft() const { return rollsleft; }
	bool isGameOver() const { return gameOver; }
	int getWinnerIndex() const { return winnerIndex; }

	const Player& getPlayer(int idx) const;
	int getPlayerCount() const { return static_cast<int>(players.size()); }
	int getLastIncomingDamageTokyo()const { return lastIncomingDamageTokyo; }

	//trenurni lica kockica(za UI prikaz)
	std::vector<DiceFace> getDiceFaces()const { return diceSet.get_lica(); }

	const std::string& getLastMessage() const { return lastMessage; }

	bool isYieldDecisionRequired() const { return yieldDecisionRequired; }
	int getPendingYieldTokyoIndex() const { return pendingYieldTokyoIndex; }
	int	getPendingYieldAttackerIndex() const { return pendingYieldAttackerIndex; }

	const std::string& getZadnjaNaziv() const { return ZadnjaNaziv; }
	const std::string& getZadnjaOpis() const { return ZadnjaOpis; }	

	void kupiRandomKartu();

	//tijek poteza

	void beginTurn();
	//odabir kockica za reroll

	void setSelectedForReroll(const std::vector<int>& indices);

	//prvi put se bacaju sve kockicem a poslije samo selectedð

	void roll();

	//potvrdi potez i izraèunaj ishod
	//yealdDecision: koristi samo ako je igraè u Tokyu primio udarac pa može izaæi

	void confirmTurn();

	void applyYieldDecision(bool yield);

	//prijeði na sljedeæeg
	void nextPlayer();

	void playCurrentAITurn();
	void dodajEnergijuTrenutnom(int iznos);
	void reset();
};