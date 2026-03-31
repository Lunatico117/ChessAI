#include "../include/Game.hpp"
#include "../include/RuleValidator.hpp"
#include "../include/Piece.hpp"

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


void Game::fillDTO(const Move& move, MoveRecordDTO& dto) {
    dto.fromRow = move.getFrom().getRow();
    dto.fromCol = move.getFrom().getCol();
    dto.toRow = move.getTo().getRow();
    dto.toCol = move.getTo().getCol();

    Piece* p = state.getBoard().getPieceAt(move.getFrom());
    if (p) {
        // Mapeamos el Enum interno a los strings que el Logger espera
        switch(p->getType()) {
        case PieceType::PAWN:   dto.pieceType = "pawn"; break;
        case PieceType::KNIGHT: dto.pieceType = "knight"; break;
        case PieceType::BISHOP: dto.pieceType = "bishop"; break;
        case PieceType::ROOK:   dto.pieceType = "rook"; break;
        case PieceType::QUEEN:  dto.pieceType = "queen"; break;
        case PieceType::KING:   dto.pieceType = "king"; break;
        }
    } else {
        dto.pieceType = "";
    }

    dto.isCapture = (state.getBoard().getPieceAt(move.getTo()) != nullptr) || (move.getType() == MoveType::EN_PASSANT);
    dto.promotion = move.getPromotionType();
    dto.moveType = move.getType();
}

void Game::executeMoveInternal(Move& move) {
    state.updateState(move);
    moveHistory.push_back(std::move(move));
    redoHistory.clear();

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
}

bool Game::internalUndo() {
    if (moveHistory.empty()) return false;

    Move lastMove = std::move(moveHistory.back());
    moveHistory.pop_back();

    state.undoState(lastMove); // Delegamos el trabajo físico al estado
    redoHistory.push_back(std::move(lastMove)); // Lo pasamos al futuro

    return true;
}

MoveResult Game::processHumanMove(Position from, Position to, PromotionType promotion) {
    MoveResult res {false, {}};
    if (gameOver) return res;

    std::vector<Move> legalMoves = getLegalMovesForPiece(from);

    for (Move& m : legalMoves) {
        if (m.getTo() == to && m.getPromotionType() == promotion) {
            fillDTO(m, res.record);
            executeMoveInternal(m);
            res.success = true;
            break;
        }
    }
    return res;
}

MoveResult Game::executeAITurn() {
    MoveResult res {false, {}};
    if (gameOver) return res;

    Color currentTurn = state.getCurrentTurn();
    std::vector<Move> legalMoves = generator.generateLegalMoves(state);
    Player* currentPlayer = (currentTurn == Color::WHITE) ? whitePlayer : blackPlayer;

    Move aiMove = currentPlayer->getMove(state, legalMoves);
    fillDTO(aiMove, res.record);
    executeMoveInternal(aiMove);

    res.success = true;
    return res;
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


bool Game::undoLastMove() {
    // 1. Reglas exclusivas del juego real
    if (gameOver && endReason == "Jaque Mate") {
        return false;
    }

    Color playerWhoMoved = (state.getCurrentTurn() == Color::WHITE) ? Color::BLACK : Color::WHITE;
    int playerIndex = (playerWhoMoved == Color::WHITE) ? 0 : 1;

    if (hasUsedUndo[playerIndex]) {
        return false;
    }

    // 2. Ejecutar la acción física
    if (internalUndo()) {
        hasUsedUndo[playerIndex] = true;
        gameOver = false;
        endReason = "";
        winnerStr = "";
        return true;
    }

    return false;
}


void Game::resetGame() {
    state = GameState(); // Reinicia el estado (y el tablero por dentro)
    gameOver = false;
    endReason = "";
    winnerStr = "";

    moveHistory.clear();
    redoHistory.clear();
    hasUsedUndo[0] = false;
    hasUsedUndo[1] = false;
}


bool Game::stepBackwardAnalysis() {
    // En análisis no importan los comodines ni si hubo Jaque Mate
    return internalUndo();
}


bool Game::stepForwardAnalysis() {
    // Si no hay nada en el futuro, no podemos avanzar
    if (redoHistory.empty()) return false;

    // Extraemos el movimiento del futuro
    Move nextMove = std::move(redoHistory.back());
    redoHistory.pop_back();

    // Aplicamos el movimiento al tablero (sin verificar reglas porque ya es legal)
    state.updateState(nextMove);

    // Lo devolvemos al historial normal del pasado
    moveHistory.push_back(std::move(nextMove));

    return true;
}
