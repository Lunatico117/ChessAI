#pragma once

#include "Piece.hpp"
#include <string>

// Declaracion anticipada, en ves de incluir "GameState.hpp" para no generar dependencias circulares
class GameState;


// Sintaxis de herencia 
class Bishop : public Piece{
    public:
        // Constructor
        Bishop(Color c);

        // Destructor 
        ~Bishop() override = default;

        // La palabra clave 'override' le dice al compilador: "Asegurate de que 
        // estoy sobreescribiendo exactamente el metodo que me pidio mi clase padre".
        std::vector<Move> getPossibleMoves(const GameState& state, const Position& currentPos) const override ;

        std::string getTypeName() const override { return "bishop"; }
};
