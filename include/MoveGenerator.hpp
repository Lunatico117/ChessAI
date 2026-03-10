#pragma once

#include <vector>
#include "Move.hpp"
#include "GameState.hpp"


class MoveGenerator{
    private:
        // Reglas especiales
        // Genera los movimientos de enroque ya que la pieza no saben que es este movimiento
        void generateCastlingMoves (const GameState& state, std::vector<Move>& moves) const;

        // Evalua la coordenada entregada por updateEnPassant para saber si es posible hacer captura al paso
        void generateEnPassantMoves(const GameState& state, std::vector<Move>& moves) const;


    public:
        // Constructor
        MoveGenerator() = default;

        // Metodo principal
        // Escanea todo el tablero y toma los movimientos Pseudolegales (incluye las reglas especiales) del jugador actual
        std::vector<Move> generatePseudoLegalMoves (const GameState& state)const;


        // Se implementara posteriormente el metodo de los movimientos legales
        // std::vector<Move> generateLegalMoves(GameState& state) const;

};
