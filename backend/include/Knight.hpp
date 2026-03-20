#pragma once

#include "Piece.hpp"
#include <string>

// Declaracion anticipada, en ves de incluir "GameState.hpp" para no generar dependencias circulares
class GameState;

// Sintaxis de herencia 
class Knight : public Piece{
    public:
        // Constructor
        Knight(Color c);

        // Destructor 
        ~Knight() override = default;

        // La palabra clave 'override' le dice al compilador: "Asegurate de que 
        // estoy sobreescribiendo exactamente el metodo que me pidio mi clase padre".
        std::vector<Move> getPossibleMoves(const GameState& state, const Position& currentPos) const override ;

        // El caballo vale 3
        int getValue() const override { return 3; }

        PieceType getType() const override {
            return PieceType::KNIGHT;
        }

        std::string getTypeName() const override { return "knight"; }
};
