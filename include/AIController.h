#pragma once

#include "IController.h"

//AIControler implementacija Icontrollera

class AIController :public IController
{
private:
	bool shouldKeepFace(DiceFace face, const Player& me)const;
public:
	AIController() = default;
	~AIController() override = default;

	std::vector<int> odaberiKockicu(
		const Game& game,
		const Player& me,
		const std::vector<DiceFace>& currentFaces,
		int rollsLeft
	) override;

	bool decideYield(
		const Game& game,
		const Player& me,
		int incomingDamage
	) override;

};