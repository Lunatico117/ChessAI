#include "../include/MatchManager.hpp"

MatchManager::MatchManager(QObject *parent) : QObject(parent) {
    reset(); // Al nacer, el juego no ha terminado
}

void MatchManager::reset() {
    m_isGameOver = false;
    m_gameOverReason = "";
    m_gameOverWinner = "";
    emit matchEnded();
}

void MatchManager::endMatch(const QString& reason, const QString& winner) {
    m_isGameOver = true;
    m_gameOverReason = reason;
    m_gameOverWinner = winner;
    emit matchEnded();
}

void MatchManager::surrender(const QString& currentTurn) {
    QString winner = (currentTurn == "white") ? "Negras" : "Blancas";
    endMatch("Rendición", "Ganador: " + winner);
}

void MatchManager::offerDraw() {
    emit drawOffered(); // Simplemente grita para que QML muestre el popup
}

void MatchManager::acceptDraw() {
    endMatch("Tablas", "Empate por acuerdo mutuo");
}

void MatchManager::declineDraw() {
    // El juego continúa, no hacemos nada que cambie el estado
}
