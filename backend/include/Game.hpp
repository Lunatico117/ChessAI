#ifndef GAME_HPP
#define GAME_HPP

#include "GameState.hpp"
#include "MoveGenerator.hpp"
#include "GameLogger.hpp"
#include "Player.hpp"
#include <string>

class Game {
private:
    GameState state;
    MoveGenerator generator;
    Player* whitePlayer;
    Player* blackPlayer;
    bool gameOver;
    std::string endReason = "";
    std::string winnerStr = "";
    // El historico de movimientos
    std::vector<Move> moveHistory;

    // Indice 0 = Blancas, Indice 1 = Negras
    // Indica si el jugador ya gasto su "Deshacer" en este turno
    bool hasUsedUndo[2];

public:
    Game(Player* white, Player* black);

    // Este metodo se usa unicamente para probar en la consola o terminal
    // Ejecuta la logica de UN solo turno. Retorna false si el juego terminó.
    bool playTurn();

    // Permite a la interfaz (main) ver el tablero para dibujarlo
    const Board& getBoard() const;

    // Metodos exclusivos para la Interfaz Grafica (GUI)

    // Devuelve los movimientos legales completos de una sola pieza (uso interno)
    std::vector<Move> getLegalMovesForPiece(Position pos);

    // Devuelve SOLO las coordenadas destino de una pieza (Para la Interfaz Gráfica)
    std::vector<Position> getLegalDestinations(Position origin);

    // Intenta ejecutar un movimiento directamente desde la interfaz
    // Movimiento para la coronacion del peon
    bool processMove(Position from, Position to, PromotionType promotion = PromotionType::NONE);
    bool isInCheck(Color color) const;

    // Metodo para saber si se debe abrir el menu de coronacion
    bool isPromotionMove(Position from, Position to) const;

    bool isGameOver() const { return gameOver; }
    std::string getEndReason() const { return endReason; }
    std::string getWinner() const { return winnerStr; }

    // Nuevo metodo para reiniciar la partida
    void resetGame();

    // Este metodo solicita el deshacer
    bool undoLastMove();

};

#endif
