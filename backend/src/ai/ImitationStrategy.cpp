#include "../include/ai/ImitationStrategy.hpp"
#include "../include/ai/RandomMoveStrategy.hpp" // Importamos la estrategia de respaldo
#include <stdexcept>

Move ImitationStrategy::calculateBestMove(const GameState& state, const std::vector<Move>& legalMoves) {
    if (legalMoves.empty()) {
        throw std::runtime_error("No hay movimientos legales para la IA.");
    }

    // 1. Obtenemos el ultimo movimiento jugado en el tablero
    const Move& lastMove = state.getLastMove();

    // 2. Calculamos las coordenadas "espejo" (simetria vertical)
    // En un tablero 8x8 (0 a 7), el espejo de la fila 'r' es '7 - r'
    int mirrorFromRow = 7 - lastMove.getFrom().getRow();
    int mirrorFromCol = lastMove.getFrom().getCol();

    int mirrorToRow = 7 - lastMove.getTo().getRow();
    int mirrorToCol = lastMove.getTo().getCol();

    Position mirrorFrom(mirrorFromRow, mirrorFromCol);
    Position mirrorTo(mirrorToRow, mirrorToCol);

    // 3. Buscamos si el movimiento espejo exacto esta dentro de los legales
    for (const Move& m : legalMoves) {
        if (m.getFrom() == mirrorFrom && m.getTo() == mirrorTo) {
            // Retornamos una copia del movimiento
            return Move(m.getFrom(), m.getTo(), m.getType(), m.getPromotionType());
        }
    }

    // 4. FALLBACK: Si el movimiento espejo no es legal (o la pieza esta bloqueada),
    // instanciamos la estrategia aleatoria y le delegamos la decision.
    RandomMoveStrategy fallback;
    return fallback.calculateBestMove(state, legalMoves);
}
