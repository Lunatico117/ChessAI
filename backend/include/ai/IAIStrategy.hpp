#pragma once
#include <vector>
#include "Move.hpp"
#include "GameState.hpp"

class IAIStrategy {
public:
    virtual ~IAIStrategy() = default;

    // Obliga a cada dificultad a recibir el estado y los movimientos legales, y devolver uno
    virtual Move calculateBestMove(const GameState& state, const std::vector<Move>& legalMoves) = 0;
};
