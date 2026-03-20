#pragma once

#include "Piece.hpp"
#include <string>

// Declaracion anticipada, en ves de incluir "GameState.hpp" para no generar dependencias circulares
class GameState;

// Sintaxis de herencia 
class Pawn : public Piece{
    
    public:
        // Constructor
        Pawn(Color c);

        // Destructor 
        ~Pawn() override = default;

        // La palabra clave 'override' le dice al compilador: "Asegurate de que 
        // estoy sobreescribiendo exactamente el metodo que me pidio mi clase padre".
        std::vector<Move> getPossibleMoves(const GameState& state, const Position& currentPos) const override ;

        // El peon vale 1
        int getValue() const override { return 1; }
        PieceType getType() const override {
            return PieceType::PAWN;
        }
        // Nombre del peon
        std::string getTypeName() const override { return "pawn"; }
};
