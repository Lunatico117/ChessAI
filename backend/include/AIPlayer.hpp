#pragma once
#include "Player.hpp"
#include "ai/IAIStrategy.hpp"
#include <memory>

class AIPlayer : public Player {
private:
    int difficultyLevel;
    std::unique_ptr<IAIStrategy> strategy;

    void setStrategyByDifficulty();

public:
    AIPlayer(Color c, int difficulty);

    // Implementa el método de tu Player.hpp
    Move getMove(const GameState& state, const std::vector<Move>& legalMoves) override;
};
