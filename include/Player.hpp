#pragma once

#include <vector>
#include "Move.hpp"
#include "GameState.hpp"
#include "Piece.hpp"

class Player{
    protected:
        Color color;

    public:
        // Constructor
        Player(Color c);

        // Destructor virtual por defecto (Es obligatorio en clases abstractas)
        virtual ~Player() = default;

        // Getter del color
        Color getColor () const;

        // Funcion virtual pura (= 0) que obliga a las clases hijas a implementarlo.
        // Recibe el estado actual y la lista de movimientos legales ya calculados.
        virtual Move getMove (const GameState& state, const std::vector<Move>& legalMoves) = 0 ;

};




