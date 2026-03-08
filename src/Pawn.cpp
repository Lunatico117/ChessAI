#include "../include/Pawn.hpp"
#include "../include/Board.hpp"

// Constructor 
Pawn::Pawn (Color c) : Piece (c){
}

std::vector<Move> Pawn::getPossibleMoves(const Board& board, const Position& currentPos) const{
    std::vector<Move> moves;
    
    int row = currentPos.getRow();
    int col = currentPos.getCol();

    // Se define la direccion y la fila de inicio para sus movimientos segun el color
    // Blancas suben (-1) Negras bajan (+1)
    int direction = (this->color == Color:: WHITE) ? -1:1;
    //Blancas inician en la fila 6 y Negras en la fila 1
    int startRow = (this-> color == Color:: WHITE) ? 6:1;
    
    // MOVIMIENTOS 
    // Una casilla adelante 
    Position oneStep(row + direction, col);
    if (board.isValidPosition(oneStep) && board.isEmpty(oneStep)){
        moves.push_back(Move(currentPos, oneStep));
    
        // Dos pasos adelante unicamente si esta en la posicion inicial 
        if (row == startRow) {
            Position twoSteps(row + 2 * direction, col);
            if(board.isEmpty(twoSteps)){
                moves.push_back(Move(currentPos, twoSteps));


            }
        }
    }


    // CAPTURAS 
    // Diagonal izquierda
    Position captureLeft(row + direction, col + 1);
    if (board.isValidPosition(captureLeft) && !board.isEmpty(captureLeft)){
        if (board.getPieceAt(captureLeft) -> getColor() != this->color){
            moves.push_back(Move(currentPos, captureLeft));
        }
    }

    // Diagonal derecha 
    Position captureRight (row + direction, col - 1); 
    if (board.isValidPosition(captureRight) && !board.isEmpty(captureRight)){
        if (board.getPieceAt(captureRight) ->getColor()!= this->color){
            moves.push_back(Move(currentPos, captureRight));
        }
    }

    // Posterior se haran las reglas de PEON AL PASO y la CORONACION 
    
    return moves;
}
