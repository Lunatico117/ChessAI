#include "../include/Pawn.hpp"
#include "../include/Board.hpp"
#include "../include/GameState.hpp"

// Constructor 
Pawn::Pawn (Color c) : Piece (c){
}

std::vector<Move> Pawn::getPossibleMoves(const GameState& state, const Position& currentPos) const{
    // Extraemos el tablero del board
    const Board& board = state.getBoard();
    // Extraemos la posicion de peon al paso
    Position epTarget = state.getEnPassantTarget();

    std::vector<Move> moves;
    
    int row = currentPos.getRow();
    int col = currentPos.getCol();

    // Se define la direccion y la fila de inicio para sus movimientos segun el color
    // Blancas suben (-1) Negras bajan (+1)
    int direction = (this->color == Color:: WHITE) ? -1:1;
    //Blancas inician en la fila 6 y Negras en la fila 1
    int startRow = (this-> color == Color:: WHITE) ? 6:1;
    
    // Fila en la que se puede coronar
    int promotionRow = (this->color == Color::WHITE) ? 0 : 7;

    // MOVIMIENTOS 
    // Una casilla adelante 
    Position oneStep(row + direction, col);
    if (board.isValidPosition(oneStep) && board.isEmpty(oneStep)){

        //Verifica si hay coronacion
        if (oneStep.getRow() == promotionRow) {
            moves.push_back(Move(currentPos, oneStep, MoveType::PROMOTION, PromotionType::QUEEN));
            moves.push_back(Move(currentPos, oneStep, MoveType::PROMOTION, PromotionType::ROOK));
            moves.push_back(Move(currentPos, oneStep, MoveType::PROMOTION, PromotionType::BISHOP));
            moves.push_back(Move(currentPos, oneStep, MoveType::PROMOTION, PromotionType::KNIGHT));
        } else {
            // Movimiento normal
            moves.push_back(Move(currentPos, oneStep));
    
            // Dos pasos adelante unicamente si esta en la posicion inicial
            if (row == startRow) {
                Position twoSteps(row + 2 * direction, col);
                if(board.isEmpty(twoSteps)){
                    moves.push_back(Move(currentPos, twoSteps));
                }
            }

        }

    }

    // CAPTURAS
    // Logica auxiliar (Lambda) para no repetir codigo en las capturas de la ultima fila
    auto addCaptureMove = [&](const Position& targetPos) {
        if (targetPos.getRow() == promotionRow) {
            moves.push_back(Move(currentPos, targetPos, MoveType::PROMOTION, PromotionType::QUEEN));
            moves.push_back(Move(currentPos, targetPos, MoveType::PROMOTION, PromotionType::ROOK));
            moves.push_back(Move(currentPos, targetPos, MoveType::PROMOTION, PromotionType::BISHOP));
            moves.push_back(Move(currentPos, targetPos, MoveType::PROMOTION, PromotionType::KNIGHT));
        } else {
            moves.push_back(Move(currentPos, targetPos));
        }
    };

    // Diagonal izquierda
    Position captureLeft(row + direction, col + 1);
    if (board.isValidPosition(captureLeft) ) {
        if ( !board.isEmpty(captureLeft)&& board.getPieceAt(captureLeft)->getColor() != this->color ){
            addCaptureMove(captureLeft);
        }
        else if(captureLeft == epTarget){
            moves.push_back(Move(currentPos, captureLeft, MoveType::EN_PASSANT));
        }
    }

    // Diagonal derecha
    Position captureRight(row + direction, col - 1);
    if (board.isValidPosition(captureRight) ) {
        if (!board.isEmpty(captureRight) &&  board.getPieceAt(captureRight)->getColor() != this->color ) {
            addCaptureMove(captureRight);
        }
        else if(captureRight == epTarget){
            moves.push_back(Move(currentPos, captureRight, MoveType::EN_PASSANT));
        }
    }

    return moves;
}

