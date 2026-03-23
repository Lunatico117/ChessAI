#pragma once

#include <QObject>
#include <QStringList>
#include <stack>
#include <QString>

class GameLogger : public QObject {
    Q_OBJECT
    // Propiedad que QML leerá para pintar la lista de movimientos
    Q_PROPERTY(QStringList moveHistoryList READ getMoveHistoryList NOTIFY moveHistoryChanged)

public:
    explicit GameLogger(QObject *parent = nullptr);

    // Getters para QML
    QStringList getMoveHistoryList() const { return m_displayList; }

    // Registra un nuevo movimiento normal en el juego
    // Si creas una nueva linea temporal con un deshacer limpia el futuro
    void logMove(const QString& pieceType, int fromRow, int fromCol, int toRow, int toCol, bool isCapture, const QString& promotedTo = "", const QString& specialMove = "", bool isCheck = false, bool isMate = false);
    void logResult(const QString& resultNotation);

    // Métodos para navegar en el modo análisis (Tus dos botones de QML)
    Q_INVOKABLE void goBackward(); // Pop de la pila pasada, Push a la pila futura
    Q_INVOKABLE void goForward();  // Pop de la pila futura, Push a la pila pasada

    // Útil para cuando se reinicia el juego
    Q_INVOKABLE void clearLog();

signals:
    // Avisa a QML que la lista visual cambió para que actualice el panel
    void moveHistoryChanged();

private:
    // LA EXIGENCIA ACADÉMICA: Estructura de doble pila
    std::stack<QString> m_pastStack;   // Historial de jugadas hechas
    std::stack<QString> m_futureStack; // Historial de jugadas deshechas (Modo análisis)

    // El espejo visual para QML
    QStringList m_displayList;

    // Metodo para convertir a notacion algebraica
    QString moveToAlgebraic(const QString& pieceType, int fromRow, int fromCol, int toRow, int toCol, bool isCapture, const QString& promotedTo = "", const QString& specialMove = "", bool isCheck = false, bool isMate = false);
};
