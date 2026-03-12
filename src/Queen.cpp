#include "../include/Queen.hpp"
#include "../include/Board.hpp"
#include "../include/GameState.hpp"


// Constructor
Queen::Queen (Color c) : Piece(c) {
}

std::vector<Move> Queen::getPossibleMoves(const GameState& state, const Position& currentPos) const{
    // Extraemos el tablero del board
    const Board& board = state.getBoard();
    std::vector <Move> moves;
    // Las 8 direcciones en las que se mueve una Dama
    // {1, 0} = Abajo, {-1, 0} = Arriba, {0, 1} = Derecha, {0, -1} = Izquierda 
    // {1, 1} = Abajo Derecha, {1, -1} = Abajo Izquierda, {-1, 1} = Arriba Derecha, {-1, -1} = Arriba Izquierda
    int directions[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

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
                // Guardamos ese último movimiento...
                if (board.getPieceAt(nextPos)->getColor() != this->getColor()) {
                    moves.push_back(Move(currentPos, nextPos));
                }
                
                // Sin importar si la pieza era nuestra o enemiga, la Dama NO puede saltarla.
                // Así que rompemos el bucle en esta direccion.
                break; 
            }

            // Avanzamos a la siguiente casilla en la MISMA direccion
            currentRow += dir[0];
            currentCol += dir[1];
        }
    }

    return moves;
}
