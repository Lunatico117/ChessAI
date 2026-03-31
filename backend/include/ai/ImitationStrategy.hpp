#pragma once
#include "ai/IAIStrategy.hpp"

class ImitationStrategy : public IAIStrategy {
public:
    Move calculateBestMove(const GameState& state, const std::vector<Move>& legalMoves) override;
};
