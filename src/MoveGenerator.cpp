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
                std::vector<Move> pieceMoves = p->getPossibleMoves(state, currentPos);

                // Extraemos cada movimiento original (por referencia) y lo TRASLADAMOS a la lista principal
                for (Move& m : pieceMoves) {
                    moves.push_back(std::move(m));
                }
            }
        }
    }

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

    // Simulacion de los movimientos
    for (Move& move : pseudoLegalMoves){

        // Simulamos el movimiento
        state.updateState(move);

        // Verificamos si el rey esta amenazado
        // (Le pasamos originalTurn porque state ya avanzo al turno del enemigo y necesita saber sobre el rey del turno actual)
        bool isKingSafe = !RuleValidator::isKingInCheck(state, originalTurn);

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



