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

    // Ejecuta la logica de UN solo turno. Retorna false si el juego terminó.
    bool playTurn();

    // Permite a la interfaz (main) ver el tablero para dibujarlo
    const Board& getBoard() const;
};

#endif
