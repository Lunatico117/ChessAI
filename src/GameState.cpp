#include "../include/GameState.hpp"
#include "../include/Pawn.hpp"
#include "../include/Rook.hpp"
#include "../include/King.hpp"

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
}


// Getters
Board& GameState::getBoard(){
    return board;
}

Color GameState::getCurrentTurn() const{
    return currentTurn;
}

Position GameState::getEnPassantTarget() const{
    return enPassantTarget;
}


bool GameState::canCastle(Color color, CastleSide side) const{
    if (color == Color::WHITE){
        return(side == CastleSide::KING_SIDE) ? wCastleK : wCastleQ;
    }
    else{
        return(side == CastleSide::KING_SIDE) ? bCastleK : bCastleQ;
    }
}

// Setters
void GameState::updateState(Move& m){
    // Guarda el estado de las reglas o banderas
    stateHistory.push({wCastleK, wCastleQ, bCastleK, bCastleQ, enPassantTarget});

}



