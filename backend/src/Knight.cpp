#include "../include/Knight.hpp"
#include "../include/Board.hpp"
#include "../include/GameState.hpp"

// Constructor
Knight::Knight(Color c) : Piece(c, PieceType::KNIGHT) {
}

std::vector<Move> Knight::getPossibleMoves(const GameState& state, const Position& currentPos) const{
    // Extraemos el tablero del board
    const Board& board = state.getBoard();
    std::vector <Move> moves;
    // Las 8 direcciones en las que se mueve un Caballo
    // {2, 1} = 2 Abajo 1 Derecha, {2, -1} = 2 Abajo 1 izquierda, {-2, 1} = 2 Arriba 1 Derecha, {-2, -1} = 2 Arriba 1 Izquierda,
    // {1, 2} = 1 Abajo 2 Derecha, {-1, 2} = 1 Abajo 2 izquierda, {1, -2} = 1 Arriba 2 Derecha, {-1, -2} = 1 Arriba 2 Izquierda
    int directions[8][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {-1, 2}, {1, -2}, {-1, -2}};

    // Iteramos para cada una de las direcciones
    for (const auto& dir : directions) {
        int currentRow = currentPos.getRow() + dir[0];
        int currentCol = currentPos.getCol() + dir[1];

        // Recorremos la direccion 
            Position nextPos(currentRow, currentCol);

            // Limite del tablero entonces se rompe el bucle 
            if (!board.isValidPosition(nextPos)) {
                continue;
            }

            // Si la casilla esta vacia, es un movimiento valido
            if (board.isEmpty(nextPos)) {
                moves.push_back(Move(currentPos, nextPos));
            } 
            // Se encuentra con una pieza
            else {
                // Si la pieza con la que chocamos es enemiga, la podemos capturar.
                if (board.getPieceAt(nextPos)->getColor() != this->getColor()) {
                    moves.push_back(Move(currentPos, nextPos));
                }
            
            }


    }

    return moves;
}
