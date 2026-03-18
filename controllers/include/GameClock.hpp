#pragma once

#include <QObject>
#include <QTimer>
#include <QString>

class GameClock : public QObject {
    Q_OBJECT
    // Propiedades que leerá QML
    Q_PROPERTY(QString whiteTimeText READ getWhiteTimeText NOTIFY timeChanged)
    Q_PROPERTY(QString blackTimeText READ getBlackTimeText NOTIFY timeChanged)

public:
    explicit GameClock(QObject *parent = nullptr);

    // Getters para QML
    QString getWhiteTimeText() const {
        return m_whiteTimeText;
    }

    QString getBlackTimeText() const {
        return m_blackTimeText;
    }

    // Controles del reloj
    void start(int minutesPerPlayer = 10);
    void stop();
    void switchTurn(const QString& currentTurn);

signals:
    // Envia la señal de que el tiempo cambio
    void timeChanged();
    // Avisa si alguien se queda sin tiempo
    void timeOut(const QString& losingColor);

private slots:
    // Los slots son funciones normales pero capaces de reaccionar a las señales (signals)
    void tick(); // Se ejecuta cada segundo

private:
    QString formatTime(int totalSeconds) const;

    QTimer* m_timer;
    int m_whiteTimeRemaining;
    int m_blackTimeRemaining;
    QString m_whiteTimeText;
    QString m_blackTimeText;
    QString m_activeTurn;
};
