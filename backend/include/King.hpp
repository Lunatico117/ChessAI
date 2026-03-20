#pragma once

#include "Piece.hpp"
#include <string>

// Declaracion anticipada, en ves de incluir "GameState.hpp" para no generar dependencias circulares
class GameState;

// Sintaxis de herencia 
class King : public Piece{
    public:
        // Constructor
        King(Color c);

        // Destructor 
        ~King() override = default;

        // La palabra clave 'override' le dice al compilador: "Asegurate de que 
        // estoy sobreescribiendo exactamente el metodo que me pidio mi clase padre".
        std::vector<Move> getPossibleMoves(const GameState& state, const Position& currentPos) const override ;

        int getValue() const override {return 1000;}

        PieceType getType() const override {
            return PieceType::KING;
        }

        std::string getTypeName() const override { return "king"; }

};
