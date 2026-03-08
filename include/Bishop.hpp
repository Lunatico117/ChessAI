#pragma once

#include "Piece.hpp"

// Sintaxis de herencia 
class Bishop : public Piece{
    public:
        // Constructor
        Bishop(Color c);

        // Destructor 
        ~Bishop() override = default;

        // La palabra clave 'override' le dice al compilador: "Asegurate de que 
        // estoy sobreescribiendo exactamente el metodo que me pidio mi clase padre".
        std::vector<Move> getPossibleMoves(const Board& board, const Position& currentPos) const override;

        // El alfil vale 3
        int getValue() const override { return 3; }
};