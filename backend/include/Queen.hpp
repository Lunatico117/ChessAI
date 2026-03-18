#pragma once

#include "Piece.hpp"
#include <string>

// Declaracion anticipada, en ves de incluir "GameState.hpp" para no generar dependencias circulares
class GameState;

// Sintaxis de herencia 
class Queen : public Piece{
    public:
        // Constructor
        Queen(Color c);

        // Destructor 
        ~Queen() override = default;

        // La palabra clave 'override' le dice al compilador: "Asegurate de que 
        // estoy sobreescribiendo exactamente el metodo que me pidio mi clase padre".
        std::vector<Move> getPossibleMoves(const GameState& state, const Position& currentPos) const override ;
        // La Dama vale 9
        int getValue() const override { return 9; }
        std::string getTypeName() const override { return "queen"; }
};
