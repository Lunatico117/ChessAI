#include "../include/AIPlayer.hpp"
#include "../include/ai/RandomMoveStrategy.hpp"

AIPlayer::AIPlayer(Color c, int difficulty) : Player(c), difficultyLevel(difficulty) {
    setStrategyByDifficulty();
}

void AIPlayer::setStrategyByDifficulty() {
    if (difficultyLevel == 1) {
        strategy = std::make_unique<RandomMoveStrategy>();
    } else {
        // Por defecto, o mientras construyes los demás niveles
        strategy = std::make_unique<RandomMoveStrategy>();
    }
}

Move AIPlayer::getMove(const GameState& state, const std::vector<Move>& legalMoves) {
    // La magia del polimorfismo: AIPlayer no sabe qué estrategia es, solo la ejecuta.
    return strategy->calculateBestMove(state, legalMoves);
}
