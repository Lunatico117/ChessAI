#pragma once

#include "Board.hpp"
#include "Move.hpp"
#include <vector>

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
    int halfMoveClock;   // Regla de los 50 movimientos
    uint64_t zobristKey; // Para un futuro usar la tabla de trasposiciones
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

        // La posicion de los Reyes
        Position whiteKingPos;
        Position blackKingPos;

        int halfMoveClock;
        uint64_t zobristKey;

        // Cambiamos stack por vector (se comporta igual usando push_back y pop_back)
        std::vector<StateInfo> stateHistory;

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
    // Metodos inline para acceso en O(1)

        // Getter para el reloj de 50 movimientos
        int getHalfMoveClock() const { return halfMoveClock; }



        // Es decir como la funcion es demasiado corta se deja en el hpp, lo cual mejora la optimizacion
        // Devuelve la posicion del rey
        Position getKingPosition(Color color) const {
            return (color == Color::WHITE) ? whiteKingPos : blackKingPos;
        }

        // Modifica la posicion del rey
        void setKingPosition(Color color, const Position& newPos) {
            if (color == Color::WHITE) {
                whiteKingPos = newPos;
            } else {
                blackKingPos = newPos;
            }
        }



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
