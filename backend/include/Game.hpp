#ifndef GAME_HPP
#define GAME_HPP

#include "GameState.hpp"
#include "MoveGenerator.hpp"
#include "Player.hpp"
#include <string>
#include "Move.hpp"

struct MoveRecordDTO {
    std::string pieceType;
    int fromRow, fromCol, toRow, toCol;
    bool isCapture;
    PromotionType promotion;
    MoveType moveType;
};

struct MoveResult {
    bool success;
    MoveRecordDTO record;
};


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
    // Estos vectores tambien se usaran para el modo analisis
    std::vector<Move> moveHistory;
    std::vector<Move> redoHistory;

    // Indice 0 = Blancas, Indice 1 = Negras
    // Indica si el jugador ya gasto su "Deshacer" en este turno
    bool hasUsedUndo[2];
    void fillDTO(const Move& move, MoveRecordDTO& dto);
    void executeMoveInternal(Move& move); // Ejecuta y guarda en el historial
    // Undo general
    bool internalUndo();

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
    MoveResult processHumanMove(Position from, Position to, PromotionType promotion = PromotionType::NONE);
    MoveResult executeAITurn();


    bool isInCheck(Color color) const;

    // Metodo para saber si se debe abrir el menu de coronacion
    bool isPromotionMove(Position from, Position to) const;

    // Este metodo solicita el deshacer
    bool undoLastMove();
    // Nuevo metodo para reiniciar la partida
    void resetGame();


    bool isGameOver() const { return gameOver; }
    std::string getEndReason() const { return endReason; }
    std::string getWinner() const { return winnerStr; }


    // Se usan en el analisis de la partida
    bool stepBackwardAnalysis();
    bool stepForwardAnalysis();


};

#endif
