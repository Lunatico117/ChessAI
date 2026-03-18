#include "../include/ChessController.hpp"
#include "../backend/include/HumanPlayer.hpp"
#include <QDebug>

ChessController::ChessController(QObject *parent)
    : QObject(parent),
    m_game(new HumanPlayer(Color::WHITE), new HumanPlayer(Color::BLACK)),
    m_boardModel(new BoardModel(this)),
    m_clock(new GameClock(this)),
    m_matchManager(new MatchManager(this)) // INICIAMOS EL ÁRBITRO
{
    // Este metodo lee la matriz y la convierte para que QML la pueda entender
    m_boardModel->updateFromGame(m_game);

    // Iniciamos el contador
    m_clock->start(10);

    // 1. Si el reloj dice "Se acabó el tiempo", avisamos al árbitro para que termine el juego
    connect(m_clock, &GameClock::timeOut, this, [this](const QString& losingColor) {
        QString winner = (losingColor == "white") ? "Ganador: Negras" : "Ganador: Blancas";
        m_matchManager->endMatch("Tiempo agotado", winner);
    });

    // 2. Si el árbitro declara el fin del juego (por tiempo, tablas o mate), detenemos el reloj
    connect(m_matchManager, &MatchManager::matchEnded, this, [this]() {
        if (m_matchManager->getIsGameOver()) {
            m_clock->stop();
        }
    });
}

void ChessController::handleSquareClick(int row, int col) {
    int index = getIndex(row, col);
    Position clickedPos(row, col);
    const Board& board = m_game.getBoard();
    Piece* clickedPiece = board.getPieceAt(clickedPos);

    // Caso 1: No tiene nada seleccionado aun

    if (m_selectedRow == -1 && m_selectedCol == -1) {

        if (clickedPiece == nullptr) return;

        QString pieceColorStr = (clickedPiece->getColor() == Color::WHITE) ? "white" : "black";
        if (pieceColorStr != m_currentTurn) return;

        m_selectedRow = row;
        m_selectedCol = col;

        m_boardModel->clearSelectionsAndHighlights();

        SquareData sqData = m_boardModel->getSquare(index);
        sqData.isSelected = true;
        m_boardModel->updateSquare(index, sqData);

        // Pedimos los movimientos legales usando tu clase Game.
        // Esto asegura que la regla de "Rey en Jaque" se respete visualmente.
        std::vector<Position> legalDestinations = m_game.getLegalDestinations(clickedPos);

        for (Position& dest : legalDestinations) {
            // Usamos los Getters de Move y Position
            int destIndex = getIndex(dest.getRow(), dest.getCol());

            SquareData destData = m_boardModel->getSquare(destIndex);
            destData.isValidMove = true;
            m_boardModel->updateSquare(destIndex, destData);
        }
    }

    // Caso 2: Ya se habia seleccionado alguna pieza
    else {
        // 1. Cancelar accion si hace clic en la misma pieza
        if (m_selectedRow == row && m_selectedCol == col) {
            m_boardModel->clearSelectionsAndHighlights();
            m_selectedRow = -1;
            m_selectedCol = -1;
            return;
        }

        SquareData clickedSqData = m_boardModel->getSquare(index);

        if (clickedSqData.isValidMove) {

            Position from(m_selectedRow, m_selectedCol);
            Position to(row, col);

            // Es una promocion

            if (m_game.isPromotionMove(from, to)) {

                // Guardamos en memoria temporal
                m_pendingFromRow = m_selectedRow;
                m_pendingFromCol = m_selectedCol;
                m_pendingToRow = row;
                m_pendingToCol = col;

                // Emitimos la señal para que QML abra el menu emergente
                emit promotionRequested();

            } else {
                // Es un movimiento normal
                bool moveSuccess = m_game.processMove(from, to);

                if (moveSuccess) {
                    m_boardModel->clearSelectionsAndHighlights();
                    m_boardModel->updateFromGame(m_game);
                    m_currentTurn = (m_currentTurn == "white") ? "black" : "white";
                    // Cambiamos el reloj cuando cambiamos el turno
                    m_clock->switchTurn(m_currentTurn);
                    emit turnChanged();
                    checkGameOver();
                } else {
                    m_boardModel->clearSelectionsAndHighlights();
                }
            }
        } else {
            m_boardModel->clearSelectionsAndHighlights();
        }

        // Siempre reiniciamos la seleccion al terminar la interaccion
        m_selectedRow = -1;
        m_selectedCol = -1;
    }
}


void ChessController::promotePendingPawn(const QString& pieceType) {
    // Traducimos el texto que nos manda QML a nuestro Enum de C++
    PromotionType promoType = PromotionType::QUEEN; // Valor por defecto seguro

    if (pieceType == "rook") {
        promoType = PromotionType::ROOK;
    }
    else if (pieceType == "bishop") {
        promoType = PromotionType::BISHOP;
    }
    else if (pieceType == "knight"){
        promoType = PromotionType::KNIGHT;
    }

    // Ejecutamos el movimiento que estaba pausado
    bool moveSuccess = m_game.processMove(
        Position(m_pendingFromRow, m_pendingFromCol),
        Position(m_pendingToRow, m_pendingToCol),
        promoType
        );

    // Si fue exitoso, limpiamos visualmente y cambiamos turno
    if (moveSuccess) {
        m_boardModel->clearSelectionsAndHighlights();
        m_boardModel->updateFromGame(m_game);
        m_currentTurn = (m_currentTurn == "white") ? "black" : "white";
        emit turnChanged();
        checkGameOver();
    }

    // Reseteamos la memoria temporal para el futuro posibles coronaciones
    m_pendingFromRow = -1;
    m_pendingFromCol = -1;
    m_pendingToRow = -1;
    m_pendingToCol = -1;
}

void ChessController::checkGameOver() {
    if (m_game.isGameOver()) {
        QString reason = QString::fromStdString(m_game.getEndReason()); // "Jaque Mate" o "Rey Ahogado"
        QString winnerStr = QString::fromStdString(m_game.getWinner());
        QString winner = winnerStr == "Empate" ? "Empate mutuo" : "Ganador: " + winnerStr;

        // Le pasamos la información al Árbitro para que él declare el fin del juego
        m_matchManager->endMatch(reason, winner);
    }
}

void ChessController::surrender() {
    m_matchManager->surrender(m_currentTurn);
}

void ChessController::offerDraw() {
    m_matchManager->offerDraw();
}

void ChessController::acceptDraw() {
    m_matchManager->acceptDraw();
}

void ChessController::declineDraw() {
    m_matchManager->declineDraw();
}

void ChessController::restartGame() {
    m_game = Game(new HumanPlayer(Color::WHITE), new HumanPlayer(Color::BLACK));
    m_currentTurn = "white";

    // Limpiamos el estado de fin de juego
    m_matchManager->reset();

    // Reiniciamos el contador de los relojs
    m_clock->start(10);

    m_boardModel->updateFromGame(m_game);
    emit turnChanged();
}
