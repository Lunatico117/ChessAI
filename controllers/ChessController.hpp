 // Evita que el archivo se incluya múltiples veces en la compilación
#pragma once

// Base fundamental de Qt, si se quiere comunicar con QML debe tener esto
#include <QObject>

// Variant es una Unión de Tipos Segura es decir
// es una caja que puede guardar un int, un QString, un QColor, etc., y sabe que tipo tiene guardado en cada momento.
// Se importan listas y mapas
#include <QVariantList>
#include <QVariantMap>
#include "../backend/include/Game.hpp"


// Al heredar QObject hereda el manejo de eventos, señales y propiedades
class ChessController : public QObject {


    // Q_OBJECT es una macro, es como una etiqueta que le permite al MOC (Meta-Object-Compiler) detectarlo y el MOC genera
    // nuevos archivos en segundo plano para facilitar la comunicacion entre objetos Qt
    Q_OBJECT


    // Q_PROPERTY es una macro que declara propiedades que el MOC puede gestionar
    // En este se define el como acceder y notificar cambios variables (Tambien las puede modifcar en algunos casos, este no es uno)
    Q_PROPERTY (QVariantList boardState READ getBoardState NOTIFY boardChanged)
    Q_PROPERTY(QString currentTurn READ getCurrentTurn NOTIFY boardChanged)

public:
    // explicit: Evita que C++ haga conversiones de tipos automaticas y
    // extrañas por accidente si alguien pasa un parametro incorrecto al crear el controlador.
    // QObject *parent = nullptr es un gestor de memoria Jerarquica de Qt, si el "padre" es destruido todos sus "hijos" igual
    explicit ChessController(QObject *parent = nullptr);

    // Q_INVOKABLE es una macro que expone los metodos de una clase C++ a QML para que sea invocada sin necesidad de invocarla como un slot
    Q_INVOKABLE bool attemptMove(int fromRow, int fromCol, int toRow, int toCol);
    Q_INVOKABLE QString getPieceIcon(int row, int col) const;

    // Es el metodo Getter que exige el Q_PROPERTY, con el const garantiza que consultar el tablero no lo modificara
    QVariantList getBoardState() const;

    QString getCurrentTurn() const { return m_currentTurn; }

    // Es una notificacion emitida por el objeto cuando hay un cambio de estado o interacion
signals:
    void boardChanged();


private:
     // Se instancia Game que es el encargado de comunicarse con la logica de las clases
    Game m_game;

    // Esta variable llevara el control de los turnos
    QString m_currentTurn = "white";

    // CACHE DE OPTIMIZACION: Almacena el tablero ya traducido a QML.
    // Evita recalcular la matriz cada vez que QML lee la propiedad.
    QVariantList m_cachedBoard;


    // Es un metodo interno que recorre la matriz y pasa esa informacion de cada casilla a QVariantList para que getBoardState
    // lo envie a la interfaz
    void updateBoardState();
};




