#pragma once

#include "GameState.hpp"
#include "Position.hpp"
#include "Piece.hpp"

class RuleValidator {
public:
    RuleValidator() = default;

    // Metodo principal
    // Devuelve true si el Rey del color del turno esta con alguna pieza amenazandolo
    bool isKingInCheck(const GameState& state, Color color) const;

private:
    // Metodos auxiliares
    // Escanea el tablero para encontrar la coordenada actual del Rey
    Position findKing(const GameState& state, Color color) const;

    // Algoritmo de Radar: Lanza rayos desde targetPos para ver si algún enemigo la ataca
    bool isSquareAttacked(const GameState& state, const Position& targetPos, Color defenderColor) const;
};
