#pragma once

#include <QObject>
#include "../backend/include/Game.hpp"
#include "BoardModel.hpp"

class ChessController : public QObject {
    Q_OBJECT
    Q_PROPERTY(BoardModel* boardModel READ getBoardModel CONSTANT)
    Q_PROPERTY(QString currentTurn READ getCurrentTurn NOTIFY turnChanged)

public:
    explicit ChessController(QObject *parent = nullptr);

    BoardModel* getBoardModel() const { return m_boardModel; }
    QString getCurrentTurn() const { return m_currentTurn; }

    Q_INVOKABLE void handleSquareClick(int row, int col);
    Q_INVOKABLE QString getPieceIcon(int row, int col) const;

signals:
    void turnChanged();

private:
    Game m_game;
    QString m_currentTurn = "white";
    BoardModel* m_boardModel;

    int m_selectedRow = -1;
    int m_selectedCol = -1;

    int getIndex(int row, int col) const { return row * 8 + col; }
    void updateBoardState();
};
