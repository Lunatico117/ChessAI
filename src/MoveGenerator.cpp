#include "../include/MoveGenerator.hpp"
#include "../include/RuleValidator.hpp"
#include "../include/Pawn.hpp"

// Metodo principal
std::vector<Move> MoveGenerator::generatePseudoLegalMoves(const GameState& state) const{
    std::vector<Move> moves;


    // Reservamos aproximandamente 45 de memoria porque es el estimado de movimientos disponibles por turno
    // Como reservamos esto el vector no se preocupa por memoria dinamica
    moves.reserve(45);

    Color turn = state.getCurrentTurn();
    const Board& board = state.getBoard();

    // Se recorre el tablero para buscar las piezas del jugador actual
    for(int r = 0; r <8; r++){
        for(int c = 0; c < 8; c++){
            Position currentPos(r, c);
            Piece* p = board.getPieceAt(currentPos);


            if(p != nullptr && p ->getColor() == turn){
                std::vector<Move> pieceMoves = p->getPossibleMoves(board, currentPos);

                // Extraemos cada movimiento original (por referencia) y lo TRASLADAMOS a la lista principal
                for (Move& m : pieceMoves) {
                    moves.push_back(std::move(m));
                }
            }
        }
    }
    generateCastlingMoves(state, moves);
    generateEnPassantMoves(state, moves);


    return moves;
}


std::vector<Move> MoveGenerator::generateLegalMoves(GameState& state) const{
    std::vector<Move> legalMoves;

    // Guardamos el turno original porque en updateState se cambiara en cuanto simulemos el movimiento
    Color originalTurn = state.getCurrentTurn();

    // Obtenemos todos los movimientos pseudolegales generados por las piezas
    std::vector<Move> pseudoLegalMoves = generatePseudoLegalMoves(state);

    // Reservamos la memoria suficiente para no deperdiciar
    legalMoves.reserve(pseudoLegalMoves.size());

    // Instanciamos el RuleValidator
    RuleValidator validator;

    // Simulacion de los movimientos
    for (Move& move : pseudoLegalMoves){

        // Simulamos el movimiento
        state.updateState(move);

        // Verificamos si el rey esta amenazado
        // (Le pasamos originalTurn porque state ya avanzo al turno del enemigo y necesita saber sobre el rey del turno actual)
        bool isKingSafe = !validator.isKingInCheck(state, originalTurn);

        // Devolvemos el movimiento
        state.undoState(move);

        // Confirmamos si el rey estaba en jaque o no
        if (isKingSafe){
            // Traslada los movimientos
            legalMoves.push_back(std::move(move));
        }
    }
    return legalMoves;
}



// Metodos privados auxiliares
void MoveGenerator:: generateCastlingMoves (const GameState& state, std::vector<Move>& moves) const {
    // Guarda las variables que se necesitan
    Color turn = state.getCurrentTurn();
    const Board& board = state.getBoard();

    // Las blancas enrocan en la fila 7, las negras en la fila 0
    int row = (turn == Color::WHITE) ? 7 : 0;

    // Enroque corto (Flanco del rey)
    if(state.canCastle(turn, CastleSide::KING_SIDE)){
        // Se verifica que las casillas entre el rey y la torre esten vacias
        if (board.getPieceAt(Position(row, 5)) == nullptr &&
            board.getPieceAt(Position(row, 6)) == nullptr) {

            moves.push_back(Move(Position(row, 4), Position(row, 6)));
        }
    }

    if(state.canCastle(turn, CastleSide::QUEEN_SIDE)){
        // Se verifica que las casillas entre el rey y la torre esten vacias
        if (board.getPieceAt(Position(row, 1)) == nullptr &&
            board.getPieceAt(Position(row, 2)) == nullptr &&
            board.getPieceAt(Position(row, 3)) == nullptr) {

            moves.push_back(Move(Position(row, 4), Position(row, 2)));
        }
    }
}


void MoveGenerator::generateEnPassantMoves(const GameState& state, std::vector<Move>& moves) const {
    //Guarda la posicion del posible peon al paso
    Position epTarget = state.getEnPassantTarget();

    // Verifica si la coordenada si esta en el tablero para descartaar que no se puede capturar al paso
    if( epTarget.getRow() == -1) return;

    // Guarda las variables que se necesitan
    Color turn = state.getCurrentTurn();
    const Board& board = state.getBoard();

    int pawnRow = (turn == Color::WHITE) ? 3 : 4;

    // Se verifica que si el peon esta a la IZQUIERDA
    // Se compara con 0 para que no busque elementos fuera del tablero
    if (epTarget.getCol()>0){
        Position leftPos(pawnRow, epTarget.getCol() - 1);
        Piece* p = board.getPieceAt(leftPos);


        if (p != nullptr && p ->getColor() == turn && dynamic_cast<Pawn*>(p) != nullptr){
            moves.push_back(Move(leftPos, epTarget));
        }
    }

    if (epTarget.getCol()<7){
        Position rightPos(pawnRow, epTarget.getCol() + 1);
        Piece* p = board.getPieceAt(rightPos);


        if (p != nullptr && p ->getColor() == turn && dynamic_cast<Pawn*>(p) != nullptr){
            moves.push_back(Move(rightPos, epTarget));
        }
    }
}

