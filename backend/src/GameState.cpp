#include "../include/GameState.hpp"
#include "../include/Pawn.hpp"
#include "../include/Rook.hpp"
#include "../include/King.hpp"
#include <cmath>

GameState:: GameState(){
    board.setupInitialPosition();
    currentTurn = Color :: WHITE;

    wCastleK = true;
    wCastleQ = true;
    bCastleK = true;
    bCastleQ = true;

    // Se le asigna una coordenada que no esta en el tablero para decirle que no hay una casilla en el tablero
    // Donde se pueda hacer la captura de peon al paso

    enPassantTarget = Position(-1, -1);


    // La posicion inicial de los Reyes
    whiteKingPos = Position(7, 4);
    blackKingPos = Position(0, 4);


}


// Getters
Board& GameState::getBoard(){
    return board;
}
// Es una sobrecarga del operador
// para que pueda pasarle el tablero a las que tengan como parametro el GameState const
const Board& GameState::getBoard() const{
    return board;
}

// Devuelve el turno actua
Color GameState::getCurrentTurn() const{
    return currentTurn;
}


bool GameState::canCastle(Color color, CastleSide side) const{
    if (color == Color::WHITE){
        return(side == CastleSide::KING_SIDE) ? wCastleK : wCastleQ;
    }
    else{
        return(side == CastleSide::KING_SIDE) ? bCastleK : bCastleQ;
    }
}


Position GameState::getEnPassantTarget() const{
    return enPassantTarget;
}


// Setters
void GameState::updateState(Move& m){

    // Verificamos que la pieza que se mueve es un rey
    Piece* p = board.getPieceAt(m.getFrom());
    if (p != nullptr && dynamic_cast<King*>(p) != nullptr) {
        setKingPosition(p->getColor(), m.getTo());
    }

    // Guarda el estado de las reglas o banderas en este turno
    stateHistory.push({wCastleK, wCastleQ, bCastleK, bCastleQ, enPassantTarget});


    // Actualizar las reglas abstractas antes de mover
    updateCastlingRights(m);
    updateEnPassant(m);


    // Se mueve la pieza
    // Aqui se aplicara la coronacion posteriormente con un condicional
    // Si m.isPromotion es verdadero se cambia el puntero inteligente del peon a una reina
    board.movePiece(m);


    // Cambiar el turno despues de mover
    currentTurn = (currentTurn == Color::WHITE)? Color::BLACK : Color::WHITE;
}

void GameState::undoState(Move& m){

    // Verificamos si la pieza que se va a devolver es un rey
    Piece* p = board.getPieceAt(m.getTo());
    if (p != nullptr && dynamic_cast<King*>(p) != nullptr) {
        setKingPosition(p->getColor(), m.getFrom());
    }


    // Devuelve el turno
    currentTurn = (currentTurn == Color::WHITE)? Color::BLACK : Color::WHITE;


    // Devuelve la pieza a su posicion anterior
    board.undoPiece(m);


    // Restaura las banderas anteriores
    if(!stateHistory.empty()){
        StateInfo previousFlags = stateHistory.top();
        stateHistory.pop();

        wCastleK = previousFlags.wCastleK;
        wCastleQ = previousFlags.wCastleQ;
        bCastleK = previousFlags.bCastleK;
        bCastleQ = previousFlags.bCastleQ;
        enPassantTarget = previousFlags.enPassantTarget;

    }
}


// Metodos privados

void GameState::updateCastlingRights(Move& m){
    // Miramos la casilla de origen antes del movimiento
    Piece* p = board.getPieceAt(m.getFrom());
    // Verificacion extra por si se mueve una casilla vacia
    if (p == nullptr) return;

    if (dynamic_cast<King*>(p) != nullptr){
        if(p->getColor() == Color::WHITE ){
            wCastleK = false;
            wCastleQ = false;
        }
        else{
            bCastleK = false;
            bCastleQ = false;
        }
    }
    else if (dynamic_cast<Rook*>(p) != nullptr){
        Position from = m.getFrom();
        if(p->getColor()== Color::WHITE){
            if (from.getRow() == 7 && from.getCol() == 0){
                wCastleQ = false;
            }
            else if (from.getRow() == 7 && from.getCol() == 7){
                wCastleK = false;
            }

        }
        else{
            if (from.getRow() == 0 && from.getCol() == 7){
                bCastleK = false;
            }
            else if (from.getRow() == 0 && from.getCol() == 0){
                bCastleQ = false;
            }
        }
    }
}

void GameState::updateEnPassant(Move& m){
    // Miramos la casilla de origen antes del movimiento
    Piece* p = board.getPieceAt(m.getFrom());
    // Verificacion extra por si se mueve una casilla vacia
    if (p == nullptr) return;

    // Asumimos por defecto que el movimiento NO genera enPassant para el siguiente turno
    enPassantTarget = Position(-1, -1);
    if (dynamic_cast<Pawn*>(p)!= nullptr){
        int rowDiff = std::abs(m.getTo().getRow() - m.getFrom().getRow());

        if (rowDiff == 2){
            // Calculamos la casilla vulnerable
            int middleRow = (m.getFrom().getRow() + m.getTo().getRow()) / 2;
            int col = m.getFrom().getCol();

            // Esta sera la casilla vulnerable en el turno del oponente
            enPassantTarget = Position(middleRow, col);
        }
    }


}





