#pragma once

#include "Piece.hpp"

// Sintaxis de herencia 
class Pawn : public Piece{
    
    public:
        // Constructor
        Pawn(Color c);

        // Destructor 
        ~Pawn() override = default;

        // La palabra clave 'override' le dice al compilador: "Asegurate de que 
        // estoy sobreescribiendo exactamente el metodo que me pidio mi clase padre".
        std::vector<Move> getPossibleMoves(const Board& board, const Position& currentPos) const override;

        // El peon vale 1
        int getValue() const override { return 1; }
};