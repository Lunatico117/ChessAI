#ifndef GAME_HPP
#define GAME_HPP

#include "GameState.hpp"
#include "MoveGenerator.hpp"
#include "Player.hpp"

class Game {
private:
    GameState state;
    MoveGenerator generator;
    Player* whitePlayer;
    Player* blackPlayer;
    bool gameOver;

public:
    Game(Player* white, Player* black);

    // Este metodo se usa unicamente para probar en la consola o terminal
    // Ejecuta la logica de UN solo turno. Retorna false si el juego terminó.
    bool playTurn();

    // Permite a la interfaz (main) ver el tablero para dibujarlo
    const Board& getBoard() const;

    // Metodos exclusivos para la Interfaz Grafica (GUI)

    // Devuelve los movimientos legales de una sola pieza
    std::vector<Move> getLegalMovesForPiece(Position pos);

    // Intenta ejecutar un movimiento directamente desde la interfaz
    bool processMove(Position from, Position to);

    bool isInCheck(Color color) const;
};

#endif
