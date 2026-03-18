#include "../include/BoardModel.hpp"

BoardModel::BoardModel(QObject *parent)
    : QAbstractListModel(parent)
{
    // Forzamos que m_board tenga 64 elementos exactos (Las casillas)
    m_board.resize(64);
}

int BoardModel::rowCount(const QModelIndex &parent) const {
    // Este le dice al compilador que no esta usando la variable "parent" pero que no lo advierta
    Q_UNUSED(parent);
    return static_cast<int>(m_board.size());
}

// Esta funcion se ejecutara muchas veces por eso debe ser eficiente y optima
// Se llamara cada vez que dibuje el tablero
QVariant BoardModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_board.size())
        return QVariant();

    const SquareData &square = m_board[index.row()];

    // Aquí traducimos la estructura de C++ para que QML la entienda
    switch (role) {
    case PieceTypeRole: return square.pieceType;
    case PieceColorRole: return square.pieceColor;
    case IsSelectedRole: return square.isSelected;
    case IsValidMoveRole: return square.isValidMove; // ¡Nuevo!
    case IsInCheckRole: return square.isInCheck;     // ¡Nuevo!
    default: return QVariant();
    }
}

// Vinculamos el nombre de c++ con el texto que QML leera
QHash<int, QByteArray> BoardModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[PieceTypeRole] = "pieceType";
    roles[PieceColorRole] = "pieceColor";
    roles[IsSelectedRole] = "isSelected";
    roles[IsValidMoveRole] = "isValidMove"; // Así lo llamaremos en QML
    roles[IsInCheckRole] = "isInCheck";     // Así lo llamaremos en QML
    return roles;
}

// Esto nos permite saber que posicion exacta cambio y enviar la señal a el QML
void BoardModel::updateSquare(int index, const SquareData& data) {
    if (index < 0 || index >= 64) return;
    m_board[index] = data;
    QModelIndex modelIndex = createIndex(index, 0);
    // Se envia la señal recibe dos parametros: desde donde
    // hasta donde cambio (en este caso, el inicio y el fin es la misma casilla)
    emit dataChanged(modelIndex, modelIndex);
}

// Limpia el jaque que antes habia, pero en ves de redibujar toda la casilla solo redibuja el color
void BoardModel::clearChecks() {
    for (size_t i = 0; i < m_board.size(); ++i) {
        if (m_board[i].isInCheck) {
            m_board[i].isInCheck = false;
            QModelIndex modelIndex = createIndex(static_cast<int>(i), 0);
            emit dataChanged(modelIndex, modelIndex, {IsInCheckRole});
        }
    }
}

// Devuelve el estado de una casilla
SquareData BoardModel::getSquare(int index) const {
    // Protección de seguridad por si piden un índice fuera del tablero
    if (index < 0 || index >= static_cast<int>(m_board.size())) {
        return SquareData();
    }
    return m_board[index];
}

// Es para cuando se crea el tablero o se destruye completamente. Cuando inicia la partida
void BoardModel::setBoard(const std::vector<SquareData>& newBoard) {
    if (newBoard.size() != 64) return;
    // Con esto QML detiene momentaneamente el renderizado
    beginResetModel();
    // Reemplazamos el antiguo vector por el nuevo
    m_board = newBoard;
    // Vuelve a renderizar
    endResetModel();
}



// Limpia las casillas que o hayan presionado una pieza o a donde se puede mover una pieza
void BoardModel::clearSelectionsAndHighlights() {
    for (size_t i = 0; i < m_board.size(); ++i) {
        if (m_board[i].isSelected || m_board[i].isValidMove) {
            m_board[i].isSelected = false;
            m_board[i].isValidMove = false;
            QModelIndex modelIndex = createIndex(static_cast<int>(i), 0);
            // Solo notificamos que cambiaron estos dos roles
            emit dataChanged(modelIndex, modelIndex, {IsSelectedRole, IsValidMoveRole});
        }
    }
}

// Funciones visuales para el tablero
void BoardModel::updateFromGame(const Game& game) {
    std::vector<SquareData> newBoard;
    newBoard.reserve(64);

    const Board& board = game.getBoard();

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Position pos(row, col);
            Piece* p = board.getPieceAt(pos);

            SquareData sq;

            if (p) {
                sq.pieceColor = (p->getColor() == Color::WHITE) ? "white" : "black";
                sq.pieceType = QString::fromStdString(p->getTypeName());

                if (sq.pieceType == "king") {
                    sq.isInCheck = game.isInCheck(p->getColor());
                }
            }
            newBoard.push_back(sq);
        }
    }
    setBoard(newBoard);
}

QString BoardModel::getPieceIcon(int row, int col) const {
    int index = (row * 8) + col;
    SquareData sq = getSquare(index);

    if (sq.pieceType == "empty") {
        return "";
    }

    QString colorPrefix = (sq.pieceColor == "white") ? "w" : "b";
    return "qrc:ui/assets/" + colorPrefix + "_" + sq.pieceType + ".svg";
}
