#include "../include/King.hpp"
#include "../include/Board.hpp"
#include "../include/GameState.hpp"

// Constructor
King::King (Color c) : Piece(c) {
}

std::vector<Move> King::getPossibleMoves(const GameState& state, const Position& currentPos) const{
    // Extraemos el tablero del board
    const Board& board = state.getBoard();
    std::vector <Move> moves;
    // Las 8 direcciones en las que se mueve un Rey
    // {1, 0} = Abajo, {-1, 0} = Arriba, {0, 1} = Derecha, {0, -1} = Izquierda 
    // {1, 1} = Abajo Derecha, {1, -1} = Abajo Izquierda, {-1, 1} = Arriba Derecha, {-1, -1} = Arriba Izquierda
    int directions[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

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

        // Si la casilla esta vacia, es un movimiento valido.
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

    int currentRow = currentPos.getRow();
    int currentCol = currentPos.getCol();

    if (currentCol == 4) {
        // Enroque Corto (King-side): Movemos a la columna 6
        if (state.canCastle(this->color, CastleSide::KING_SIDE)) {
            // Verificamos que las dos casillas intermedias estén vacías
            if (board.isEmpty(Position(currentRow, 5)) && board.isEmpty(Position(currentRow, 6))) {
                moves.push_back(Move(currentPos, Position(currentRow, 6), MoveType::CASTLING));
            }
        }

        // Enroque Largo (Queen-side): Movemos a la columna 2
        if (state.canCastle(this->color, CastleSide::QUEEN_SIDE)) {
            // Verificamos que las tres casillas intermedias estén vacías
            if (board.isEmpty(Position(currentRow, 3)) && board.isEmpty(Position(currentRow, 2)) && board.isEmpty(Position(currentRow, 1))) {
                moves.push_back(Move(currentPos, Position(currentRow, 2), MoveType::CASTLING));
            }
        }
    }


    return moves;
}
