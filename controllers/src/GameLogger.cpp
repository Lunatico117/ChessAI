#include "../include/GameLogger.hpp"

GameLogger::GameLogger(QObject *parent) : QObject(parent) {
    // El constructor no necesita hacer nada especial,
    // las pilas y la lista nacen vacías por defecto.
}


void GameLogger::logMove(const MoveRecordDTO& dto, bool isCheck, bool isMate) {
    QString special = "";
    if (dto.moveType == MoveType::CASTLING) {
        // Columna 6 es 'g' (enroque corto), columna 2 es 'c' (enroque largo)
        special = (dto.toCol == 6) ? "O-O" : "O-O-O";
    }

    QString promo = "";
    switch(dto.promotion) {
    case PromotionType::QUEEN:  promo = "Q"; break;
    case PromotionType::ROOK:   promo = "R"; break;
    case PromotionType::BISHOP: promo = "B"; break;
    case PromotionType::KNIGHT: promo = "N"; break;
    default: promo = ""; break;
    }

    // Llamamos a tu método original enviando los datos traducidos
    logMove(QString::fromStdString(dto.pieceType),
            dto.fromRow, dto.fromCol, dto.toRow, dto.toCol,
            dto.isCapture, promo, special, isCheck, isMate);
}

// El logMove que traduce (solo llama al de arriba)
void GameLogger::logMove(const QString& pieceType, int fromRow, int fromCol, int toRow, int toCol, bool isCapture, const QString& promotedTo, const QString& specialMove, bool isCheck, bool isMate) {
    // Le pasamos el nuevo parámetro al traductor
    QString sanNotation = moveToAlgebraic(pieceType, fromRow, fromCol, toRow, toCol, isCapture, promotedTo, specialMove, isCheck, isMate);

    m_pastStack.push(sanNotation);
    m_futureStack = std::stack<QString>();
    m_displayList.append(sanNotation);

    emit moveHistoryChanged();
}

// Este se encarga del final de la partida
void GameLogger::logResult(const QString& resultNotation) {
    m_displayList.append(resultNotation);
    m_pastStack.push(resultNotation);
    m_futureStack = std::stack<QString>();
    emit moveHistoryChanged();
}


void GameLogger::goBackward() {
    // Si no hay pasado, no podemos retroceder
    if (m_pastStack.empty()) return;

    // 1. Extraemos el elemento de la cima (podría ser un movimiento normal o un resultado)
    QString item = m_pastStack.top();
    m_pastStack.pop();
    m_futureStack.push(item);

    // Lo quitamos visualmente de la lista de QML
    if (!m_displayList.isEmpty()) {
        m_displayList.removeLast();
    }

    // 2. LA MAGIA: Si el elemento que acabamos de sacar era un RESULTADO,
    // significa que tenemos que sacar un elemento más (el movimiento real que lo causó).
    if (item == "1-0" || item == "0-1" || item == "1/2-1/2") {

        if (!m_pastStack.empty()) {
            QString realMove = m_pastStack.top();
            m_pastStack.pop();
            m_futureStack.push(realMove);

            // Lo quitamos también del QML
            if (!m_displayList.isEmpty()) {
                m_displayList.removeLast();
            }
        }
    }

    emit moveHistoryChanged();
}

void GameLogger::goForward() {
    // Si no hay futuro guardado, no podemos avanzar
    if (m_futureStack.empty()) return;

    // 1. Sacamos el movimiento del futuro y lo devolvemos al pasado
    QString move = m_futureStack.top();
    m_futureStack.pop();
    m_pastStack.push(move);

    // Lo volvemos a poner en la lista de QML
    m_displayList.append(move);

    // 2. LA MAGIA INVERSA: Verificamos si lo que quedó AHORA en la cima del futuro
    // es el RESULTADO que acompañaba a este movimiento.
    if (!m_futureStack.empty()) {
        QString nextItem = m_futureStack.top();

        // Si es un resultado, lo pasamos al historial inmediatamente en el mismo turno
        if (nextItem == "1-0" || nextItem == "0-1" || nextItem == "1/2-1/2") {
            m_futureStack.pop();
            m_pastStack.push(nextItem);

            // Lo ponemos en QML
            m_displayList.append(nextItem);
        }
    }

    emit moveHistoryChanged();
}

void GameLogger::clearLog() {
    // Vaciamos todo cuando se reinicia la partida
    m_pastStack = std::stack<QString>();
    m_futureStack = std::stack<QString>();
    m_displayList.clear();

    emit moveHistoryChanged();
}

QString GameLogger::moveToAlgebraic(const QString& pieceType, int fromRow, int fromCol, int toRow, int toCol, bool isCapture, const QString& promotedTo, const QString& specialMove, bool isCheck, bool isMate) {

    if (specialMove == "O-O" || specialMove == "O-O-O") {
        return specialMove;
    }

    QString notation = "";
    QString pType = pieceType; // Por seguridad

    // 1. Inicial de la pieza (Caballo = N, Alfil = B, Torre = R, Reina = Q, Rey = K)
    if (pType == "knight") notation += "N";
    else if (pType == "bishop") notation += "B";
    else if (pType == "rook") notation += "R";
    else if (pType == "queen") notation += "Q";
    else if (pType == "king") notation += "K";
    // Si es "pawn" (peón), no se pone letra.

    // 2. ¿Hubo captura?
    if (isCapture) {
        // Regla especial: Si un peón captura, se debe poner la columna de origen (ej. exd5)
        if (pType == "pawn" || pType == "") {
            notation += QString(QChar('a' + fromCol));
        }
        notation += "x"; // La 'x' indica captura
    }

    // 3. Casilla destino (Columna: a-h)
    notation += QString(QChar('a' + toCol));

    // 4. Casilla destino (Fila: 1-8)
    // Como la matriz inicia arriba en 0 se pone asi, si fuera al contrario se pondria "notation += QString::number(toRow + 1);"
    notation += QString::number(8 - toRow);

    // 5. Promocion
    if (!promotedTo.isEmpty()) {
        notation += "=" + promotedTo;
    }

    // 6. Jaque o Jaque Mate
    if (isMate) notation += "#";
    else if (isCheck) notation += "+";

    return notation;
}
