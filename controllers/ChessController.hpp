#pragma once

#include <QObject>
#include "../backend/include/Game.hpp"
#include "BoardModel.hpp"

class ChessController : public QObject {
    Q_OBJECT
    Q_PROPERTY(BoardModel* boardModel READ getBoardModel CONSTANT)
    Q_PROPERTY(QString currentTurn READ getCurrentTurn NOTIFY turnChanged)
    // Propiedades para el fin de juego
    Q_PROPERTY(bool isGameOver READ getIsGameOver NOTIFY gameOverStateChanged)
    Q_PROPERTY(QString gameOverReason READ getGameOverReason NOTIFY gameOverStateChanged)
    Q_PROPERTY(QString gameOverWinner READ getGameOverWinner NOTIFY gameOverStateChanged)

public:
    explicit ChessController(QObject *parent = nullptr);

    BoardModel* getBoardModel() const { return m_boardModel; }
    QString getCurrentTurn() const { return m_currentTurn; }

    bool getIsGameOver() const { return m_isGameOver; }
    QString getGameOverReason() const { return m_gameOverReason; }
    QString getGameOverWinner() const { return m_gameOverWinner; }

    Q_INVOKABLE void handleSquareClick(int row, int col);
    Q_INVOKABLE QString getPieceIcon(int row, int col) const;
    // Este invokable se usara cuando elija la pieza para la coronacion
    Q_INVOKABLE void promotePendingPawn(const QString& pieceType);
    // Este invokable se usara para rendirse
    Q_INVOKABLE void surrender();
    // Estos invokable se usara para las tablas
    Q_INVOKABLE void offerDraw();
    Q_INVOKABLE void acceptDraw();
    Q_INVOKABLE void declineDraw();

    // Este invokable se usara para reiniciar el tablero
    Q_INVOKABLE void restartGame();


signals:
    void turnChanged();
    // Señal para avisar que hay una coronacion
    void promotionRequested();

    // Señal para la finalizacion del juego
    void gameOverStateChanged();
signals:
    // Ofrecimiento de tablas
    void drawOffered();

private:

    Game m_game;
    QString m_currentTurn = "white";
    BoardModel* m_boardModel;

    // Variables de estado para el fin del juego
    bool m_isGameOver = false;
    QString m_gameOverReason = "";
    QString m_gameOverWinner = "";

    int m_selectedRow = -1;
    int m_selectedCol = -1;

    // Memoria temporal para pausar la coronacion
    int m_pendingFromRow = -1;
    int m_pendingFromCol = -1;
    int m_pendingToRow = -1;
    int m_pendingToCol = -1;

    int getIndex(int row, int col) const { return row * 8 + col; }
    void updateBoardState();

    void checkGameOver();
};
