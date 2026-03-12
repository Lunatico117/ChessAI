#pragma once

#include "Piece.hpp"

// Sintaxis de herencia 
class King : public Piece{
    public:
        // Constructor
        King(Color c);

        // Destructor 
        ~King() override = default;

        // La palabra clave 'override' le dice al compilador: "Asegurate de que 
        // estoy sobreescribiendo exactamente el metodo que me pidio mi clase padre".
        std::vector<Move> getPossibleMoves(const Board& board, const Position& currentPos) const override;

        int getValue() const override {return 100;}
};
