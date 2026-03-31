#include "../include/ai/RandomMoveStrategy.hpp"
#include <random>
#include <stdexcept>

Move RandomMoveStrategy::calculateBestMove(const GameState& state, const std::vector<Move>& legalMoves) {
    if (legalMoves.empty()) {
        throw std::runtime_error("No hay movimientos legales para la IA.");
    }

    // Configuracion del generador de numeros aleatorios
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, legalMoves.size() - 1);

    int randomIndex = distrib(gen);

    // Como legalMoves es const, debemos devolver una copia del movimiento seleccionado.
    return Move(legalMoves[randomIndex].getFrom(),
                legalMoves[randomIndex].getTo(),
                legalMoves[randomIndex].getType(),
                legalMoves[randomIndex].getPromotionType());
}
