#pragma once
#include "AIController.h"
#include <vector>
#include <optional>

class HumanController : public IController
{
private: 
	std::optional<std::vector<int>> pendingRerollIndices;
	std::optional<bool> pendingYieldDecision;
public:
	HumanController() = default;
	~HumanController() override = default;

	void setRerollIndices(const std::vector<int>& indices);
	void setYieldDecision(bool yield);

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