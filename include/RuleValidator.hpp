#ifndef RULEVALIDATOR_HPP
#define RULEVALIDATOR_HPP

#include "GameState.hpp"
#include "Position.hpp"

class RuleValidator {
public:
    // Ahora son estáticos y no tienen "const" al final
    static bool isKingInCheck(const GameState& state, Color color);
    static bool isSquareAttacked(const GameState& state, const Position& targetPos, Color defenderColor);
};

#endif
