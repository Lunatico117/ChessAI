#pragma once

#include "Board.hpp"
#include "Move.hpp"
#include <stack>

enum  class CastleSide {
    KING_SIDE,
    QUEEN_SIDE
};

struct StateInfo
{
    bool wCastleK; // whiteCanCastleKingSide
    bool wCastleQ; // whiteCanCastleQueenSide
    bool bCastleK; // blackCanCastleKingSide
    bool bCastleQ; // blackCanCastleQueenSide
    Position enPassantTarget;
};


class GameState {
    private: 
        // El tablero fisico que se mostrara en el GUI
        Board board;

        // Determinara el turno
        Color currentTurn;


        // Detecta el estado de estas jugadas para poder usarlas
        bool wCastleK; // whiteCanCastleKingSide
        bool wCastleQ; // whiteCanCastleQueenSide
        bool bCastleK; // blackCanCastleKingSide
        bool bCastleQ; // blackCanCastleQueenSide

        Position enPassantTarget;

        // Es el historial de reglas para tener en cuenta cuando se realiza un Undo
        std::stack<StateInfo> stateHistory;

        // Se detectan los movimientos del rey o la torre para cambiar el estado del enroque 
        void updateCastlingRights(Move& m);

        // Método para calcular si se habilita el peón al paso
        void updateEnPassant(Move& m);
    
    public:
        GameState();
        

        // Getters
    
        // Pasa el tablero por referencia para no duplicarlo y consumir mas memoria
        Board& getBoard();

        const Board& getBoard() const;

        // Retorna el turno actual
        Color getCurrentTurn () const;
        
        // Retorna si es posible realizar el enroque 
        bool canCastle (Color color, CastleSide side) const;

        // Permite saber donde se puede capturar al paso actualmente
        Position getEnPassantTarget() const;

        // Setters

        // Si aplica un movimiento cambia los estados, cambia el turno y mueve la pieza 
        void updateState (Move& m);

        // Deshace los estados cambiados, devuelve el turno y devuelve la pieza
        void undoState (Move& m);
};
