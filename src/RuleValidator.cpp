#include "../include/RuleValidator.hpp"
#include "../include/King.hpp"
#include "../include/Pawn.hpp"
#include "../include/Knight.hpp"
#include "../include/Rook.hpp"
#include "../include/Bishop.hpp"
#include "../include/Queen.hpp"

// Metodo principal
bool RuleValidator::isKingInCheck(const GameState& state, Color color) const{

    // Lectura rapida O(1) desde el cache del estado!
    Position kingPos = state.getKingPosition(color);

    // Una verificacion extra por si por algun bug el rey no esta en el tablero
    if(kingPos.getRow() == -1){
        return false;
    }

    return isSquareAttacked(state, kingPos, color);
}

bool RuleValidator::isSquareAttacked(const GameState& state, const Position& targetPos, Color defenderColor)const{
    // Obtiene el tablero a partir de un metodo de GameState
    const Board& board = state.getBoard();
    // Obtiene fila y columna de la posicion
    int targetRow = targetPos.getRow();
    int targetCol = targetPos.getCol();

    Color enemyColor = (defenderColor == Color::WHITE) ? Color::BLACK: Color::WHITE;



    // Amenaza del Peón
    // Los peones enemigos atacan desde la fila de la que vienen.
    int pawnDir = (defenderColor == Color::WHITE) ? -1 : 1;
    int pawnAttackRow = targetRow + pawnDir;

    if (pawnAttackRow >= 0 && pawnAttackRow < 8) {
        // Revisamos la diagonal izquierda
        if (targetCol - 1 >= 0) {
            Piece* p = board.getPieceAt(Position(pawnAttackRow, targetCol - 1));
            if (p != nullptr && p->getColor() == enemyColor && dynamic_cast<Pawn*>(p) != nullptr){
                return true;
            }
        }

        // Revisamos la diagonal derecha
        if (targetCol + 1 < 8) {
            Piece* p = board.getPieceAt(Position(pawnAttackRow, targetCol + 1));
            if (p != nullptr && p->getColor() == enemyColor && dynamic_cast<Pawn*>(p) != nullptr){
                return true;
            }
        }
    }


    // Amenaza de torres y damas en vertical y horizontal
    int straightMoves[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}}; // Arriba, Abajo, Izquierda, Derecha

    // Iteramos para cada una de las direcciones
    for (auto& dir : straightMoves) {
        for (int i = 1; i < 8; ++i) { // Expandimos "el rayo" hasta chocar
            int checkRow = targetRow + dir[0] * i;
            int checkCol = targetCol + dir[1] * i;

            // Verificacion extra para que no se salga del tablero
            if (checkRow < 0 || checkRow >= 8 || checkCol < 0 || checkCol >= 8) break;

            // Verifica la casilla
            Piece* p = board.getPieceAt(Position(checkRow, checkCol));
            if (p != nullptr) {
                // Si chocamos contra una Torre o Reina enemiga, nos están atacando
                if (p->getColor() == enemyColor && (dynamic_cast<Rook*>(p) != nullptr || dynamic_cast<Queen*>(p) != nullptr)) {
                    return true;
                }
                break; // Si chocamos contra cualquier OTRA pieza se detiene "el rayo"
            }
        }
    }


    // Amenaza de alfiles y damas en diagonal
    int diagonalMoves[4][2] = {{-1,-1}, {-1,1}, {1,-1}, {1,1}}; // Las 4 diagonales
    for (auto& dir : diagonalMoves) {
        for (int i = 1; i < 8; ++i) {
            int checkRow = targetRow + dir[0] * i;
            int checkCol = targetCol + dir[1] * i;

             // Verificacion extra para que no se salga del tablero
            if (checkRow < 0 || checkRow >= 8 || checkCol < 0 || checkCol >= 8) break;

            // Verifica la casilla
            Piece* p = board.getPieceAt(Position(checkRow, checkCol));
            if (p != nullptr) {
                if (p->getColor() == enemyColor && (dynamic_cast<Bishop*>(p) != nullptr || dynamic_cast<Queen*>(p) != nullptr)) {
                    return true;
                }
                break; // Si chocamos contra cualquier OTRA pieza se detiene "el rayo"
            }
        }
    }


    // Amenaza del rey enemigo
    int kingMoves[8][2] = {{-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1}};
    for (auto& move : kingMoves) {
        int checkRow = targetRow + move[0];
        int checkCol = targetCol + move[1];

        if (checkRow >= 0 && checkRow < 8 && checkCol >= 0 && checkCol < 8) {
            Piece* p = board.getPieceAt(Position(checkRow, checkCol));
            if (p != nullptr && p->getColor() == enemyColor && dynamic_cast<King*>(p) != nullptr) {
                return true;
            }
        }
    }


    // Amenaza del Caballo
    int knightMoves[8][2] = {{-2,-1}, {-2,1}, {-1,-2}, {-1,2}, {1,-2}, {1,2}, {2,-1}, {2,1}};
    for (auto& move : knightMoves) {
        int checkRow = targetRow + move[0];
        int checkCol = targetCol + move[1];

        if (checkRow >= 0 && checkRow < 8 && checkCol >= 0 && checkCol < 8) { // Validar bordes
            Piece* p = board.getPieceAt(Position(checkRow, checkCol));
            if (p != nullptr && p->getColor() == enemyColor && dynamic_cast<Knight*>(p) != nullptr) {
                return true;
            }
        }
    }


    // Si usamos todos los radares y no encontramos enemigos el rey esta a salvo
    return false;



}
