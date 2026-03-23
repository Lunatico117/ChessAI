#pragma once

#include <QObject>
#include "../backend/include/Game.hpp"
#include "BoardModel.hpp"
#include "GameClock.hpp"
#include "MatchManager.hpp"
#include "GameLogger.hpp"

class ChessController : public QObject {
    Q_OBJECT
    Q_PROPERTY(BoardModel* boardModel READ getBoardModel CONSTANT)
    Q_PROPERTY(QString currentTurn READ getCurrentTurn NOTIFY turnChanged)
    // Tiempo por jugador
    Q_PROPERTY(GameClock* clock READ getClock CONSTANT)
    // Arbitro
    Q_PROPERTY(MatchManager* match READ getMatch CONSTANT)
    Q_PROPERTY(GameLogger* logger READ getLogger CONSTANT)


public:
    explicit ChessController(QObject *parent = nullptr);

    // Getters
    BoardModel* getBoardModel() const { return m_boardModel; }
    QString getCurrentTurn() const { return m_currentTurn; }

    GameClock* getClock() const { return m_clock; }

    MatchManager* getMatch() const { return m_matchManager; }

    GameLogger* getLogger() const { return m_logger; }

    Q_INVOKABLE void handleSquareClick(int row, int col);

    // Este invokable se usara cuando elija la pieza para la coronacion
    Q_INVOKABLE void promotePendingPawn(const QString& pieceType);

    // Este se usara para deshacer un movimiento
    Q_INVOKABLE void requestUndo();

    // Estos se usaran para el control del juego mediante el arbitro
    Q_INVOKABLE void surrender();
    Q_INVOKABLE void offerDraw();
    Q_INVOKABLE void acceptDraw();
    Q_INVOKABLE void declineDraw();

    // Este invokable se usara para reiniciar el tablero
    Q_INVOKABLE void restartGame();


signals:
    void turnChanged();
    // Señal para avisar que hay una coronacion
    void promotionRequested();

    void undoSuccessful();


private:

    Game m_game;
    QString m_currentTurn = "white";
    BoardModel* m_boardModel;

    GameClock* m_clock;

    MatchManager* m_matchManager;

    GameLogger* m_logger;

    int m_selectedRow = -1;
    int m_selectedCol = -1;

    // Memoria temporal para pausar la coronacion
    int m_pendingFromRow = -1;
    int m_pendingFromCol = -1;
    int m_pendingToRow = -1;
    int m_pendingToCol = -1;

    int getIndex(int row, int col) const { return row * 8 + col; }

    void checkGameOver();
};
