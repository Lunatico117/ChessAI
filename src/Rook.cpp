#include "../include/Rook.hpp"
#include "../include/Board.hpp"
#include "../include/GameState.hpp"

// Constructor
Rook::Rook (Color c) : Piece(c) {
}

std::vector<Move> Rook::getPossibleMoves(const GameState& state, const Position& currentPos) const{
    // Extraemos el tablero del board
    const Board& board = state.getBoard();
    std::vector <Move> moves;
    // Las 4 direcciones en las que se mueve una torre
    // {1, 0} = Abajo, {-1, 0} = Arriba, {0, 1} = Derecha, {0, -1} = Izquierda
    int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    // Iteramos para cada una de las direcciones
    for (const auto& dir : directions) {
        int currentRow = currentPos.getRow() + dir[0];
        int currentCol = currentPos.getCol() + dir[1];

        // Recorremos la direccion hasta chocar con algo (Ficha aliada o enemiga, limite del tablero)
        while (true) {
            Position nextPos(currentRow, currentCol);

            // Limite del tablero entonces se rompe el bucle 
            if (!board.isValidPosition(nextPos)) {
                break; 
            }

            // Si la casilla esta vacia, es un movimiento valido. Lo guardamos y seguimos avanzando.
            if (board.isEmpty(nextPos)) {
                moves.push_back(Move(currentPos, nextPos));
            } 
            // Se encuentra con una pieza
            else {
                // Si la pieza con la que chocamos es enemiga, la podemos capturar.
                if (board.getPieceAt(nextPos)->getColor() != this->getColor()) {
                    moves.push_back(Move(currentPos, nextPos));
                }
                
                // Sin importar si la pieza era nuestra o enemiga, la torre NO puede saltarla.
                // Asi que rompemos el bucle en esta direccion.
                break; 
            }

            // Avanzamos a la siguiente casilla en la MISMA direccion
            currentRow += dir[0];
            currentCol += dir[1];
        }
    }

    return moves;
}
