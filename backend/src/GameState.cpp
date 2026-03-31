#include "../include/GameState.hpp"
#include "../include/Pawn.hpp"
#include "../include/Rook.hpp"
#include "../include/King.hpp"
#include <cmath>

GameState:: GameState()
: lastMovePlayed(Position(0,0), Position(0,0))
{
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

    // Inicializamos en 0
    halfMoveClock = 0;
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

    Piece* p = board.getPieceAt(m.getFrom());
    if (p == nullptr) return; // Seguridad

    // 1. Guardar el estado actual ANTES de cambiar cualquier cosa
    stateHistory.push_back({wCastleK, wCastleQ, bCastleK, bCastleQ, enPassantTarget, halfMoveClock, zobristKey,
    Move(lastMovePlayed.getFrom(), lastMovePlayed.getTo(), lastMovePlayed.getType(), lastMovePlayed.getPromotionType())});

    // 2. Lógica del Reloj de 50 movimientos (halfMoveClock)
    // Se resetea a 0 si se mueve un peon o si hay una captura. Si no, suma 1.
    if (m.isCapture() || p->getType() == PieceType::PAWN) {
        halfMoveClock = 0;
    } else {
        halfMoveClock++;
    }

    // 3. Rastreo del Rey
    if (p->getType() == PieceType::KING) {
        setKingPosition(p->getColor(), m.getTo());
    }

    // 4. Actualizar las reglas abstractas antes de mover
    updateCastlingRights(m);
    updateEnPassant(m);

    // 5. Mover la pieza en el tablero fisico
    board.movePiece(m);

    // 6. Cambiar turno
    currentTurn = (currentTurn == Color::WHITE) ? Color::BLACK : Color::WHITE;

    // Guardamos el ultimo movimiento
    lastMovePlayed = Move(m.getFrom(), m.getTo(), m.getType(), m.getPromotionType());
}

void GameState::undoState(Move& m){

    // Verificamos si la pieza que se va a devolver es un rey
    Piece* p = board.getPieceAt(m.getTo());
    if (p != nullptr && p->getType() == PieceType::KING) {
        setKingPosition(p->getColor(), m.getFrom());
    }


    // Devuelve el turno
    currentTurn = (currentTurn == Color::WHITE)? Color::BLACK : Color::WHITE;


    // Devuelve la pieza a su posicion anterior
    board.undoPiece(m);


    // Restaura las banderas anteriores
    if(!stateHistory.empty()){

        StateInfo previousState = std::move(stateHistory.back());
        stateHistory.pop_back();

        wCastleK = previousState.wCastleK;
        wCastleQ = previousState.wCastleQ;
        bCastleK = previousState.bCastleK;
        bCastleQ = previousState.bCastleQ;
        enPassantTarget = previousState.enPassantTarget;
        halfMoveClock = previousState.halfMoveClock; // Restauramos reloj
        zobristKey = previousState.zobristKey;       // Restauramos hash
        lastMovePlayed = std::move(previousState.lastMovePlayed);
    }
}


// Metodos privados

void GameState::updateCastlingRights(Move& m){
    // Si capturan una torre en su casilla inicial el enroque se pierde
    Position to = m.getTo();
    if (to.getRow() == 0 && to.getCol() == 0) bCastleQ = false;
    else if (to.getRow() == 0 && to.getCol() == 7) bCastleK = false;
    else if (to.getRow() == 7 && to.getCol() == 0) wCastleQ = false;
    else if (to.getRow() == 7 && to.getCol() == 7) wCastleK = false;


    // Miramos la casilla de origen antes del movimiento
    Piece* p = board.getPieceAt(m.getFrom());
    // Verificacion extra por si se mueve una casilla vacia
    if (p == nullptr) return;

    if (p->getType() == PieceType::KING){
        if(p->getColor() == Color::WHITE ){
            wCastleK = false;
            wCastleQ = false;
        }
        else{
            bCastleK = false;
            bCastleQ = false;
        }
    }
    else if (p->getType() == PieceType::ROOK){
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
    if (p->getType() == PieceType::PAWN){
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





