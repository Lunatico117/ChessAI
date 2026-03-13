#include "../include/Game.hpp"
#include "../include/RuleValidator.hpp"
#include <iostream>

Game::Game(Player* white, Player* black)
    // Como el Player se guarda como un puntero tenemos que asignarle un valor
    // A generator no ya que no pasamos un puntero, reservamos memoria para
    : whitePlayer(white), blackPlayer(black), gameOver(false) {
}

bool Game::playTurn() {
    if (gameOver) return false;

    Color currentTurn = state.getCurrentTurn();
    std::vector<Move> legalMoves = generator.generateLegalMoves(state);

    // Se verifica si hay Jaque Mate o Rey Ahogado
    if (legalMoves.empty()) {
        if (RuleValidator::isKingInCheck(state, currentTurn)) {
            std::cout << "\n¡JAQUE MATE! Gana el jugador "
                      << (currentTurn == Color::WHITE ? "NEGRO" : "BLANCO") << ".\n";
        } else {
            std::cout << "\n¡TABLAS por Rey Ahogado!\n";
        }
        gameOver = true;
        return false; // El juego termina
    }

    // Se pide el movimiento al jugador correspondiente
    Player* currentPlayer = (currentTurn == Color::WHITE) ? whitePlayer : blackPlayer;
    Move chosenMove = currentPlayer->getMove(state, legalMoves);

    // Se ejecuta el movimiento
    state.updateState(chosenMove);

    return true; // El juego continua
}

const Board& Game::getBoard() const {
    return state.getBoard();
}
