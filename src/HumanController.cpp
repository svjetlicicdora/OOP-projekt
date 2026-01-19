#include "HumanController.h"
#include <stdexcept>

void HumanController::setRerollIndices(const std::vector<int>& indices)
{
    pendingRerollIndices = indices;
}

void HumanController::setYieldDecision(bool yield)
{
    pendingYieldDecision = yield;
}

std::vector<int> HumanController::odaberiKockicu(
    const Game& /*game*/,
    const Player& /*me*/,
    const std::vector<DiceFace>& /*currentFaces*/,
    int rollsLeft
)
{
    if (rollsLeft <= 0)
        return {};

    if (!pendingRerollIndices.has_value())
        throw std::runtime_error("Niste odabrali kockice za ponovno bacanje.");

    std::vector<int> result = *pendingRerollIndices;
    pendingRerollIndices.reset();
    return result;
}

bool HumanController::decideYield(
    const Game& /*game*/,
    const Player& /*me*/,
    int /*incomingDamage*/
)
{
    if (!pendingYieldDecision.has_value())
        throw std::runtime_error("Niste donijeli odluku o izlasku iz Tokyja.");

    bool result = *pendingYieldDecision;
    pendingYieldDecision.reset();
    return result;
}
