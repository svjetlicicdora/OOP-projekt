#include "AIController.h"
#include "Game.h"
#include "Player.h"



bool AIController::shouldKeepFace(DiceFace face, const Player& me)const
{
	//uvijek zadržava šapu
	if (face == DiceFace::Claw)
		return true;

	//2. pravilo- zadržava srce ako ima manje od 4 srca 

	if (face == DiceFace::Heart)
	{
		if (me.get_lp() <= 4 && me.get_lp() < 12)
			return true;
		return false;
	}

	//3. pravilo uvijek sadržava energiju

	if (face == DiceFace::Lightning)
		return true;

	return false;
}

std::vector<int> AIController::odaberiKockicu(
    const Game&,
    const Player& me,
    const std::vector<DiceFace>& currentFaces,
    int rollsLeft
)
{
    if (rollsLeft <= 0)
        return {};

    std::vector<int> indices;

    // reroll svih kockica koje AI ne želi zadržati
    for (int i = 0; i < static_cast<int>(currentFaces.size()); ++i)
    {
        if (!shouldKeepFace(currentFaces[i], me))
        {
            indices.push_back(i);
        }
    }

    return indices;
}


bool AIController::decideYield(
	const Game& game,
	const Player& me,
	int incomingDamage
)
{
	int lpAfter = me.get_lp() - incomingDamage;
	if (lpAfter >= 4)
		return true;  //izlazi iz Tokija
	return false; //ostaje u Tokiju
}