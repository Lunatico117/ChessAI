#pragma once

#include <vector>
#include "Move.hpp"
#include "GameState.hpp"


class MoveGenerator{

    public:
        // Constructor
        MoveGenerator() = default;

        // Metodo principal
        // Escanea todo el tablero y toma los movimientos Pseudolegales (incluye las reglas especiales) del jugador actual
        std::vector<Move> generatePseudoLegalMoves (const GameState& state)const;


        // Se implementara posteriormente el metodo de los movimientos legales
        std::vector<Move> generateLegalMoves(GameState& state) const;

};
