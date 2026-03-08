#pragma once

#include "Piece.hpp"

// Sintaxis de herencia 
class Rook : public Piece{
    public:
        // Constructor
        Rook(Color c);

        // Destructor 
        ~Rook() override = default;

        // La palabra clave 'override' le dice al compilador: "Asegurate de que 
        // estoy sobreescribiendo exactamente el metodo que me pidio mi clase padre".
        std::vector<Move> getPossibleMoves(const Board& board, const Position& currentPos) const override;

        // La torre vale 5 
        int getValue() const override { return 5; }
    };