#include "ChessController.hpp"
#include "../backend/include/HumanPlayer.hpp"
#include "../backend/include/Pawn.hpp"
#include "../backend/include/Rook.hpp"
#include "../backend/include/Knight.hpp"
#include "../backend/include/Bishop.hpp"
#include "../backend/include/Queen.hpp"
#include "../backend/include/King.hpp"
#include <QDebug>

ChessController::ChessController(QObject *parent)
    : QObject(parent),
    m_game(new HumanPlayer(Color::WHITE), new HumanPlayer(Color::BLACK)),
    // Esto lo que hace es que si el BoardModel se destruye entonces el m_boardModel Tambien
    m_boardModel(new BoardModel(this))
{
    // Este metodo lee la matriz y la convierte para que QML la pueda entender
    updateBoardState();
}

void ChessController::updateBoardState() {
    std::vector<SquareData> newBoard;
    // Se reserva una lista vacia de 64 casillas
    newBoard.reserve(64);

    // Obtenemos el tablero real del motor (Por referencia)
    const Board& board = m_game.getBoard();

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Position pos(row, col);
            Piece* p = board.getPieceAt(pos);

            SquareData sq;

            if (p) {
                sq.pieceColor = (p->getColor() == Color::WHITE) ? "white" : "black";
                // De esta manera se puede saber que pieza es
                if (dynamic_cast<Pawn*>(p)) sq.pieceType = "pawn";
                else if (dynamic_cast<Rook*>(p)) sq.pieceType = "rook";
                else if (dynamic_cast<Knight*>(p)) sq.pieceType = "knight";
                else if (dynamic_cast<Bishop*>(p)) sq.pieceType = "bishop";
                else if (dynamic_cast<Queen*>(p)) sq.pieceType = "queen";
                else if (dynamic_cast<King*>(p)) {
                    sq.pieceType = "king";
                    sq.isInCheck = m_game.isInCheck(p->getColor());
                }
            }
            newBoard.push_back(sq);
        }
    }
    m_boardModel->setBoard(newBoard);
}

void ChessController::handleSquareClick(int row, int col) {
    int index = getIndex(row, col);
    Position clickedPos(row, col);
    const Board& board = m_game.getBoard();
    Piece* clickedPiece = board.getPieceAt(clickedPos);

    // No tiene nada seleccionado aun

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
        std::vector<Move> legalMoves = m_game.getLegalMovesForPiece(clickedPos);

        for (Move& move : legalMoves) {
            // Usamos los Getters de Move y Position
            int destIndex = getIndex(move.getTo().getRow(), move.getTo().getCol());

            SquareData destData = m_boardModel->getSquare(destIndex);
            destData.isValidMove = true;
            m_boardModel->updateSquare(destIndex, destData);
        }
    }
    // Ya se habia seleccionado alguna pieza
    else {
        if (m_selectedRow == row && m_selectedCol == col) {
            m_boardModel->clearSelectionsAndHighlights();
            m_selectedRow = -1;
            m_selectedCol = -1;
            return;
        }

        SquareData clickedSqData = m_boardModel->getSquare(index);

        if (clickedSqData.isValidMove) {

            // Usamos processMove(), que es el metodo en Game.cpp
            // Retorna 'true' si el movimiento se encontro y se ejecuto correctamente.
            bool moveSuccess = m_game.processMove(Position(m_selectedRow, m_selectedCol), Position(row, col));

            if (moveSuccess) {
                m_boardModel->clearSelectionsAndHighlights();
                updateBoardState(); // Redibuja todo por si hubo capturas o enroques

                // Alternamos el turno visual solo si el movimiento en el backend fue exitoso
                m_currentTurn = (m_currentTurn == "white") ? "black" : "white";
                emit turnChanged();
            } else {
                // Si processMove devuelve false (ej. juego terminado o error), solo limpiamos
                m_boardModel->clearSelectionsAndHighlights();
            }
        } else {
            m_boardModel->clearSelectionsAndHighlights();
        }

        // Siempre reiniciamos la selección al terminar la interacción
        m_selectedRow = -1;
        m_selectedCol = -1;
    }
}

QString ChessController::getPieceIcon(int row, int col) const {
    // 1. Convertimos la fila y columna en el índice plano (0 a 63)
    int index = getIndex(row, col);

    // 2. Leemos la información visual de esa casilla desde nuestro modelo
    SquareData sq = m_boardModel->getSquare(index);

    // 3. Si la casilla está vacía, devolvemos un string vacío (QML lo hará invisible)
    if (sq.pieceType == "empty") {
        return "";
    }

    // 4. Determinamos el prefijo del color ("w" para white, "b" para black)
    QString colorPrefix = (sq.pieceColor == "white") ? "w" : "b";

    // 5. Construimos y devolvemos la ruta exacta del recurso
    // Ejemplo de resultado: "qrc:/assets/w_pawn.svg"
    return "qrc:ui/assets/" + colorPrefix + "_" + sq.pieceType + ".svg";
}







