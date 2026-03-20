#include "../include/Game.hpp"
#include "../include/RuleValidator.hpp"
#include <iostream>

Game::Game(Player* white, Player* black)
    // Como el Player se guarda como un puntero tenemos que asignarle un valor
    // A generator no ya que no pasamos un puntero, reservamos memoria para
    : whitePlayer(white), blackPlayer(black), gameOver(false) {
    hasUsedUndo[0] = false;
    hasUsedUndo[1] = false;
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

    // Guarda el movimiento y resetea el derecho del rival para deshacer
    moveHistory.push_back(std::move(chosenMove));
    Color nextTurn = state.getCurrentTurn();
    hasUsedUndo[nextTurn == Color::WHITE ? 0 : 1] = false;

    return true; // El juego continua
}

const Board& Game::getBoard() const {
    return state.getBoard();
}

std::vector<Move> Game::getLegalMovesForPiece(Position pos) {
    // Generamos TODOS los movimientos legales del turno actual
    std::vector<Move> allLegalMoves = generator.generateLegalMoves(state);
    std::vector<Move> pieceMoves;

    // Filtramos solo los que pertenecen a la pieza que el usuario toco
    for (Move& m : allLegalMoves) {
        if (m.getFrom() == pos) {
            pieceMoves.push_back(std::move(m));
        }
    }
    return pieceMoves;
}


bool Game::processMove(Position from, Position to, PromotionType promotion) {
    if (gameOver) return false;

    // Solo pedimos los movimientos legales de la pieza que estamos tocando
    std::vector<Move> legalMoves = getLegalMovesForPiece(from);
    bool moveFound = false;

    for (Move& m : legalMoves) {
        if (m.getTo() == to) { // No necesitamos comparar m.getFrom() == from porque la lista ya está filtrada

            if (m.getType() == MoveType::PROMOTION) {
                if (m.getPromotionType() == promotion) {
                    state.updateState(m);
                    moveHistory.push_back(std::move(m));
                    moveFound = true;
                    break;
                }
            } else {
                state.updateState(m);
                moveHistory.push_back(std::move(m));
                moveFound = true;
                break;
            }
        }
    }

    // Si no se encontro el movimiento, lo rechazamos
    if (!moveFound) {
        return false;
    }


    // Verificamos si este movimiento causo Jaque Mate o Tablas al rival
    Color nextTurn = state.getCurrentTurn();
    hasUsedUndo[nextTurn == Color::WHITE ? 0 : 1] = false;
    std::vector<Move> nextMoves = generator.generateLegalMoves(state);

    if (nextMoves.empty()) {
        if (RuleValidator::isKingInCheck(state, nextTurn)) {
            endReason = "Jaque Mate";
            winnerStr = (nextTurn == Color::WHITE) ? "Negras" : "Blancas";
        } else {
            endReason = "Rey Ahogado";
            winnerStr = "Empate";
        }
        gameOver = true;
    }

    return true; // Movimiento exitoso
}


bool Game::isInCheck(Color color) const {
    return RuleValidator::isKingInCheck(state, color);
}


bool Game::isPromotionMove(Position from, Position to) const {
    Piece* p = state.getBoard().getPieceAt(from);
    if (p == nullptr) return false;

    // Llamada polimórfica: la pieza (sea la que sea) sabe cómo moverse
    std::vector<Move> possibleMoves = p->getPossibleMoves(state, from);

    for (const Move& m : possibleMoves) {
        if (m.getTo() == to && m.getType() == MoveType::PROMOTION) {
            return true;
        }
    }
    return false;
}



void Game::resetGame() {
    state = GameState(); // Reinicia el estado (y el tablero por dentro)
    gameOver = false;
    endReason = "";
    winnerStr = "";

    moveHistory.clear();
    hasUsedUndo[0] = false;
    hasUsedUndo[1] = false;
}

std::vector<Position> Game::getLegalDestinations(Position origin) {
    std::vector<Position> destinations;
    // Usamos tu funcion existente para obtener todos los movimientos completos
    std::vector<Move> moves = getLegalMovesForPiece(origin);

    // Filtramos y guardamos unicamente la posición de destino
    for(const Move& m : moves) {
        destinations.push_back(m.getTo());
    }
    return destinations;
}


bool Game::undoLastMove() {
    if (moveHistory.empty()) return false; // No hay nada en el historial

    // No permite deshacer cuando se haya acabado la partida
    if (gameOver && endReason == "Jaque Mate") {
        return false; // Bloquea la acción por completo
    }
    if (moveHistory.empty()) return false;

    // ¿De quien fue el ultimo turno jugado? (El jugador que quiere deshacer)
    Color playerWhoMoved = (state.getCurrentTurn() == Color::WHITE) ? Color::BLACK : Color::WHITE;
    int playerIndex = (playerWhoMoved == Color::WHITE) ? 0 : 1;

    // Verificamos si ya uso su comodin en este ciclo
    if (hasUsedUndo[playerIndex]) {
        return false;
    }

    // Extraemos el ultimo movimiento de forma segura
    Move lastMove = std::move(moveHistory.back());
    moveHistory.pop_back();

    // Revertimos la logica del estado, el turno y devolvemos la pieza capturada
    state.undoState(lastMove);

    // Marcamos que ya uso su comodin por este turno
    hasUsedUndo[playerIndex] = true;

    // Detalle importante: Si el juego habia terminado por Mate, al deshacer lo reanudamos
    gameOver = false;
    endReason = "";
    winnerStr = "";

    return true;
}
