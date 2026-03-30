#pragma once
#include "IAIStrategy.hpp"

class RandomMoveStrategy : public IAIStrategy {
public:
    Move calculateBestMove(const GameState& state, const std::vector<Move>& legalMoves) override;
};
