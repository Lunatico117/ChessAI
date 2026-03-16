#include "ChessController.hpp"
// Se llama para instanciar a los jugadores
#include "../backend/include/HumanPlayer.hpp"
// Incluimos las piezas para usar el dynamic_cast
#include "../backend/include/Pawn.hpp"
#include "../backend/include/Rook.hpp"
#include "../backend/include/Knight.hpp"
#include "../backend/include/Bishop.hpp"
#include "../backend/include/Queen.hpp"
#include "../backend/include/King.hpp"
#include <QDebug> // Para imprimir en la consola de Qt



// Constructor
ChessController::ChessController(QObject *parent)
    : QObject(parent),
    // Al instanciar Game se deben tambien agregar dos instancias de jugador (Posteriormente sera una IA y un jugador)
    // Se reserva dinamicamente con new y despues las borraremos con delete en Game para evitar fugas de memoria
    m_game(new HumanPlayer(Color::WHITE), new HumanPlayer(Color::BLACK))
{
    // Al nacer el controlador, generamos la primera "foto" del tablero inicial
    updateBoardState();

}

// Cuando la interfaz grafica necesite saber como esta el tablero le entragamos el m_cacheBoard
// Asi no tenemos que mirar todo el tablero de nuevo
QVariantList ChessController::getBoardState() const {
    return m_cachedBoard;
}

bool ChessController::attemptMove(int fromRow, int fromCol, int toRow, int toCol) {
    Position from(fromRow, fromCol);
    Position to(toRow, toCol);

    // Le delegamos todo el trabajo a la fachada Game
    bool success = m_game.processMove(from, to);

    if (success) {
        // Cambia el turno de manera segura
        m_currentTurn = (m_currentTurn == "white") ? "black" : "white";
        updateBoardState(); // Actualiza el caché visual (Fase 2 completada)
    }

    return success; // Retorna true a QML para hacer un sonido de "Pieza movida"
}

QString ChessController::getPieceIcon(int row, int col) const {
    // 1. Obtenemos el tablero de solo lectura (O(1))
    const Board& board = m_game.getBoard();

    // 2. Leemos el puntero de la pieza en esa posición
    Piece* piece = board.getPieceAt(Position(row, col));

    // 3. Si es nullptr, la casilla está vacía
    if (!piece) {
        return "";
    }

    // 4. Determinamos el prefijo del color ("w_" o "b_")
    QString colorPrefix = (piece->getColor() == Color::WHITE) ? "w" : "b";
    QString pieceName = "";

    // 5. Polimorfismo: Averiguamos qué pieza es usando dynamic_cast
    // Tal como lo diseñaste en tu GameState para la captura al paso
    if (dynamic_cast<Pawn*>(piece)) pieceName = "pawn";
    else if (dynamic_cast<Rook*>(piece)) pieceName = "rook";
    else if (dynamic_cast<Knight*>(piece)) pieceName = "knight";
    else if (dynamic_cast<Bishop*>(piece)) pieceName = "bishop";
    else if (dynamic_cast<Queen*>(piece)) pieceName = "queen";
    else if (dynamic_cast<King*>(piece)) pieceName = "king";

    // 6. Retornamos la ruta exacta que QML necesita
    // Ejemplo: "qrc:/ui/assets/w_pawn.png"
    return "qrc:/ui/assets/" + colorPrefix + "_" + pieceName + ".svg";
}

void ChessController::updateBoardState() {
    QVariantList newList;
    // Se usa el metodo para obtener el tablero
    const Board& board = m_game.getBoard();

    // Recorremos la matriz 8x8
    // Se puede hacer esto ya que la IA no lo usara, por lo tanto no daña la optimizacion
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {

            // Usamos la clase Position
            Position pos(row, col);
            Piece* p = board.getPieceAt(pos);

            // QVariantMap es como un diccionario de Python o un objeto JSON
            QVariantMap squareData;

            if (p == nullptr) {
                squareData["type"] = "empty";
                squareData["pieceColor"] = "none";
            } else {
                // Guardamos el color
                squareData["pieceColor"] = (p->getColor() == Color::WHITE) ? "white" : "black";

                // Identificamos el tipo de pieza usando dynamic_cast
                if (dynamic_cast<Pawn*>(p)) squareData["type"] = "pawn";
                else if (dynamic_cast<Rook*>(p)) squareData["type"] = "rook";
                else if (dynamic_cast<Knight*>(p)) squareData["type"] = "knight";
                else if (dynamic_cast<Bishop*>(p)) squareData["type"] = "bishop";
                else if (dynamic_cast<Queen*>(p)) squareData["type"] = "queen";
                else if (dynamic_cast<King*>(p)) squareData["type"] = "king";
                else squareData["type"] = "unknown";
            }

            // Agregamos esta casilla a la lista plana
            newList.append(squareData);
        }
    }

    // Actualizamos el caché y gritamos a QML que se redibuje
    m_cachedBoard = newList;
    emit boardChanged();
}
































