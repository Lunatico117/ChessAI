#pragma once
#include "GameState.hpp"

class Evaluator {
public:
    // Esta es la función principal que llamará el Minimax
    static int evaluate(const GameState& state);

private:
    // Bonus por posición (Piece-Square Tables)
    static int getPositionalBonus(PieceType type, Color color, const Position& pos);
};
