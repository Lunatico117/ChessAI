#pragma once

#include <QAbstractListModel>
#include <QVariant>
#include <vector>
#include "../backend/include/Game.hpp"

// Estructura interna para representar TODO el estado visual de una casilla
struct SquareData {
    QString pieceType = "empty"; // "pawn", "knight", "empty", etc.
    QString pieceColor = "none"; // "white", "black", "none"
    bool isSelected = false;     // Para resaltar la pieza que el usuario tocó
    bool isValidMove = false;    // Para dibujar el puntito/bolita de "movimiento posible"
    bool isInCheck = false;      // Para teñir de rojo tenue la casilla si hay Jaque
};


// Como la clase BoardModel hereda public de QAbstractListModel entonces es un modelo de lista que
// Qt ya sabe como conectar con la vista grafica QML
class BoardModel : public QAbstractListModel {
    Q_OBJECT

public:
    // Roles que QML podrá leer
    enum SquareRoles {
        // El framework tiene reservados los valores enteros del 0 al 255 para directivas de renderizado del propio motor gráfico
        // La documentación de Qt exige que cualquier rol personalizado (enum) inicie después de ese límite
        // Para evitar colisiones de API o que QML malinterprete tus datos como comandos de dibujo nativos
        PieceTypeRole = Qt::UserRole + 1,
        PieceColorRole,
        IsSelectedRole,
        IsValidMoveRole,
        IsInCheckRole
    };

    // Evita conversiones de tipo implicitas no deseadas por parte del compilador
    explicit BoardModel(QObject *parent = nullptr);

    // Metodos obligatorios heredados del QAbstractListModel
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;



    // Metodos para el Controller
    void setBoard(const std::vector<SquareData>& newBoard);
    void updateSquare(int index, const SquareData& data);

    // Metodo para la
    Q_INVOKABLE QString getPieceIcon(int row, int col) const;
    void updateFromGame(const Game& game);

    // Limpia selecciones y bolitas de movimiento
    void clearSelectionsAndHighlights();

    // Devuelve el estado de una casilla
    SquareData getSquare(int index) const;

    // Quita el estado de jaque de todas las casillas
    void clearChecks();

private:
    std::vector<SquareData> m_board;
};
