#include "../include/GameClock.hpp"

GameClock::GameClock(QObject *parent) : QObject(parent), m_timer(new QTimer(this)) {
    // Conectamos el timer a la función tick
    // Cada vez que el timer de la señal el tick se ejecuta
    connect(m_timer, &QTimer::timeout, this, &GameClock::tick);
}

// Inicia el reloj
void GameClock::start(int minutesPerPlayer) {
    m_whiteTimeRemaining = minutesPerPlayer * 60;
    m_blackTimeRemaining = minutesPerPlayer * 60;
    m_activeTurn = "white"; // Las blancas siempre empiezan

    m_whiteTimeText = formatTime(m_whiteTimeRemaining);
    m_blackTimeText = formatTime(m_blackTimeRemaining);
    emit timeChanged();

    m_timer->start(1000); // 1000 ms = 1 segundo
}

// Detiene el reloj
void GameClock::stop() {
    m_timer->stop();
}

// Cambia de turno es decir cambia de reloj
void GameClock::switchTurn(const QString& currentTurn) {
    m_activeTurn = currentTurn;
}

// Resta 1 al tiempo y despues lo convierte en el formato adecuado
// Revisa si se acabo el tiempo de alguno y da la señal de a quien se le acabo
void GameClock::tick() {
    if (m_activeTurn == "white") {
        m_whiteTimeRemaining--;
        m_whiteTimeText = formatTime(m_whiteTimeRemaining);
        if (m_whiteTimeRemaining <= 0) {
            stop();
            emit timeOut("white");
        }
    } else {
        m_blackTimeRemaining--;
        m_blackTimeText = formatTime(m_blackTimeRemaining);
        if (m_blackTimeRemaining <= 0) {
            stop();
            emit timeOut("black");
        }
    }
    emit timeChanged();
}

// Es el que formatea la cantidad de segundos a un reloj con minutos y segundos|
QString GameClock::formatTime(int totalSeconds) const {
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    // Es una plantilla para saber como se debe enviar el formato del tiempo
    return QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
}
