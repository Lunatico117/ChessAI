#pragma once

#include <QObject>
#include <QString>

class MatchManager : public QObject {
    Q_OBJECT
    // Propiedades expuestas a QML
    Q_PROPERTY(bool isGameOver READ getIsGameOver NOTIFY matchEnded)
    Q_PROPERTY(QString gameOverReason READ getGameOverReason NOTIFY matchEnded)
    Q_PROPERTY(QString gameOverWinner READ getGameOverWinner NOTIFY matchEnded)

public:
    explicit MatchManager(QObject *parent = nullptr);

    // Getters
    bool getIsGameOver() const { return m_isGameOver; }
    QString getGameOverReason() const { return m_gameOverReason; }
    QString getGameOverWinner() const { return m_gameOverWinner; }

    // Acciones de los jugadores (Invocables desde QML)
    Q_INVOKABLE void surrender(const QString& currentTurn);
    Q_INVOKABLE void offerDraw();
    Q_INVOKABLE void acceptDraw();
    Q_INVOKABLE void declineDraw();

    // Controles internos para C++
    void endMatch(const QString& reason, const QString& winner);
    void reset();

signals:
    void matchEnded();   // Avisa que el estado de isGameOver cambió
    void drawOffered();  // Avisa a QML que debe abrir el popup de tablas

private:
    bool m_isGameOver;
    QString m_gameOverReason;
    QString m_gameOverWinner;
};
