#pragma once
#include "../include/Player.hpp"
#include <string>

class HumanPlayer : public Player {
public:
    HumanPlayer(Color c);

    // Sobrescribimos el método virtual de la clase padre
    Move getMove(const GameState& state, const std::vector<Move>& legalMoves) override;

private:
    // Función auxiliar para convertir texto como "e2" a un objeto Position(6, 4)
    Position parsePosition(const std::string& input) const;
};
