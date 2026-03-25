#include "../include/ChessController.hpp"
#include "../backend/include/HumanPlayer.hpp"
#include <QDebug>

ChessController::ChessController(QObject *parent)
    : QObject(parent),
    m_game(new HumanPlayer(Color::WHITE), new HumanPlayer(Color::BLACK)),
    m_boardModel(new BoardModel(this)),
    m_clock(new GameClock(this)),
    m_matchManager(new MatchManager(this)), // INICIAMOS EL ÁRBITRO
    m_logger(new GameLogger(this))
{
    // Este metodo lee la matriz y la convierte para que QML la pueda entender
    m_boardModel->updateFromGame(m_game);

    // Iniciamos el contador
    m_clock->start(10);

    // 1. Si el reloj dice "Se acabó el tiempo", avisamos al árbitro para que termine el juego
    connect(m_clock, &GameClock::timeOut, this, [this](const QString& losingColor) {
        QString winner = (losingColor == "white") ? "Ganador: Negras" : "Ganador: Blancas";
        m_matchManager->endMatch("Tiempo agotado", winner);
    });

    // 2. Si el árbitro declara el fin del juego (por tiempo, tablas o mate), detenemos el reloj
    connect(m_matchManager, &MatchManager::matchEnded, this, [this]() {
        if (m_matchManager->getIsGameOver()) {
            m_clock->stop();
        }
    });
}

void ChessController::handleSquareClick(int row, int col) {

    if (m_isAnalysisMode) {
        return;
    }

    int index = getIndex(row, col);
    Position clickedPos(row, col);
    const Board& board = m_game.getBoard();
    Piece* clickedPiece = board.getPieceAt(clickedPos);

    // Caso 1: No tiene nada seleccionado aun

    if (m_selectedRow == -1 && m_selectedCol == -1) {

        if (clickedPiece == nullptr) return;

        QString pieceColorStr = (clickedPiece->getColor() == Color::WHITE) ? "white" : "black";
        if (pieceColorStr != m_currentTurn) return;

        m_selectedRow = row;
        m_selectedCol = col;

        m_boardModel->clearSelectionsAndHighlights();

        SquareData sqData = m_boardModel->getSquare(index);
        sqData.isSelected = true;
        m_boardModel->updateSquare(index, sqData);

        // Pedimos los movimientos legales usando tu clase Game.
        // Esto asegura que la regla de "Rey en Jaque" se respete visualmente.
        std::vector<Position> legalDestinations = m_game.getLegalDestinations(clickedPos);

        for (Position& dest : legalDestinations) {
            // Usamos los Getters de Move y Position
            int destIndex = getIndex(dest.getRow(), dest.getCol());

            SquareData destData = m_boardModel->getSquare(destIndex);
            destData.isValidMove = true;
            m_boardModel->updateSquare(destIndex, destData);
        }
    }

    // Caso 2: Ya se habia seleccionado alguna pieza
    else {
        // 1. Cancelar accion si hace clic en la misma pieza
        if (m_selectedRow == row && m_selectedCol == col) {
            m_boardModel->clearSelectionsAndHighlights();
            m_selectedRow = -1;
            m_selectedCol = -1;
            return;
        }

        SquareData clickedSqData = m_boardModel->getSquare(index);

        if (clickedSqData.isValidMove) {

            Position from(m_selectedRow, m_selectedCol);
            Position to(row, col);

            // Es una promocion

            if (m_game.isPromotionMove(from, to)) {

                // Guardamos en memoria temporal
                m_pendingFromRow = m_selectedRow;
                m_pendingFromCol = m_selectedCol;
                m_pendingToRow = row;
                m_pendingToCol = col;

                // Emitimos la señal para que QML abra el menu emergente
                emit promotionRequested();

            } else {
                // 1. Averiguamos que pieza se está moviendo (usando el índice de la selección original)
                int selectedIndex = getIndex(m_selectedRow, m_selectedCol);
                QString movingPiece = m_boardModel->getSquare(selectedIndex).pieceType;

                // 2. Averiguamos si hay una pieza en la casilla destino (para saber si es captura)
                bool isCapture = (board.getPieceAt(to) != nullptr);

                bool isKingsideCastle = m_game.isKingsideCastle(from, to);
                bool isQueensideCastle = m_game.isQueensideCastle(from, to);
                bool isEnPassant = m_game.isEnPassant(from, to);

                QString specialMove = "";
                if (isKingsideCastle) specialMove = "O-O";
                else if (isQueensideCastle) specialMove = "O-O-O";

                // Si es peón al paso, forzamos que isCapture sea true
                if (isEnPassant) isCapture = true;
                // Es un movimiento normal
                bool moveSuccess = m_game.processMove(from, to);

                if (moveSuccess) {

                    bool isMate = (m_game.isGameOver() && m_game.getEndReason() == "Jaque Mate");
                    bool isCheck = false;

                    //Se debe traducir de String a un ENUM para poder enviarlo con los parametros que necesita
                    Color opponentColor = (m_currentTurn == "white") ? Color::BLACK : Color::WHITE;
                    if (!isMate) {
                        isCheck = m_game.isInCheck(opponentColor); // <-- DEBES IMPLEMENTAR/USAR TU MÉTODO AQUÍ
                    }

                    // Pasar isCheck e isMate al Logger
                    m_logger->logMove(movingPiece, m_selectedRow, m_selectedCol, row, col, isCapture, "", specialMove, isCheck, isMate);
                    m_boardModel->clearSelectionsAndHighlights();
                    m_boardModel->updateFromGame(m_game);
                    m_currentTurn = (m_currentTurn == "white") ? "black" : "white";
                    // Cambiamos el reloj cuando cambiamos el turno
                    m_clock->switchTurn(m_currentTurn);
                    emit turnChanged();
                    checkGameOver();
                } else {
                    m_boardModel->clearSelectionsAndHighlights();
                }
            }
        } else {
            m_boardModel->clearSelectionsAndHighlights();
        }

        // Siempre reiniciamos la seleccion al terminar la interaccion
        m_selectedRow = -1;
        m_selectedCol = -1;
    }
}


void ChessController::promotePendingPawn(const QString& pieceType) {
    // Traducimos el texto que nos manda QML a nuestro Enum de C++
    PromotionType promoType = PromotionType::QUEEN; // Valor por defecto seguro
    QString promoNotation = "Q";

    if (pieceType == "rook") {
        promoType = PromotionType::ROOK;
        promoNotation = "R";
    }
    else if (pieceType == "bishop") {
        promoType = PromotionType::BISHOP;
        promoNotation = "B";
    }
    else if (pieceType == "knight"){
        promoType = PromotionType::KNIGHT;
        promoNotation = "N";
    }

    // Averiguamos si había una pieza en el destino antes de mover
    const Board& board = m_game.getBoard();
    Position toPos(m_pendingToRow, m_pendingToCol);
    bool isCapture = (board.getPieceAt(toPos) != nullptr);

    // Ejecutamos el movimiento que estaba pausado
    bool moveSuccess = m_game.processMove(
        Position(m_pendingFromRow, m_pendingFromCol),
        Position(m_pendingToRow, m_pendingToCol),
        promoType
        );

    // Si fue exitoso, limpiamos visualmente y cambiamos turno
    if (moveSuccess) {
        bool isMate = (m_game.isGameOver() && m_game.getEndReason() == "Jaque Mate");
        bool isCheck = false;

        if (!isMate) {
            Color opponentColor = (m_currentTurn == "white") ? Color::BLACK : Color::WHITE;
            isCheck = m_game.isInCheck(opponentColor);
        }
        m_logger->logMove("pawn", m_pendingFromRow, m_pendingFromCol, m_pendingToRow, m_pendingToCol, isCapture, promoNotation, "", isCheck, isMate);        m_boardModel->clearSelectionsAndHighlights();
        m_boardModel->updateFromGame(m_game);
        m_currentTurn = (m_currentTurn == "white") ? "black" : "white";
        emit turnChanged();
        checkGameOver();
    }

    // Reseteamos la memoria temporal para el futuro posibles coronaciones
    m_pendingFromRow = -1;
    m_pendingFromCol = -1;
    m_pendingToRow = -1;
    m_pendingToCol = -1;
}


void ChessController::requestUndo() {

    if (m_isAnalysisMode) {
        return;
    }
    // Le pedimos al Backend que intente deshacer
    if (m_game.undoLastMove()) {

        // El logger retrocede sus pilas
        m_logger->goBackward();
        // Revertimos la variable de turno local
        m_currentTurn = (m_currentTurn == "white") ? "black" : "white";

        // Le decimos al reloj que vuelva a cambiar de lado
        m_clock->switchTurn(m_currentTurn);

        // Limpiamos cualquier clic fantasma que el usuario haya dejado a medias
        m_selectedRow = -1;
        m_selectedCol = -1;
        m_boardModel->clearSelectionsAndHighlights();

        // Sincronizamos la matriz visual con el backend
        m_boardModel->updateFromGame(m_game);

        // Notificamos a QML
        emit turnChanged();
        emit undoSuccessful();

        qDebug() << "Movimiento deshecho con éxito.";
    } else {
        qDebug() << "No se puede deshacer: Historial vacío o límite por turno alcanzado.";
    }
}


void ChessController::surrender() {
    m_matchManager->surrender(m_currentTurn);
    QString result = (m_currentTurn == "white") ? "0-1" : "1-0";
    m_logger->logResult(result);
}


void ChessController::offerDraw() {
    m_matchManager->offerDraw();
}


void ChessController::acceptDraw() {
    m_matchManager->acceptDraw();
    m_logger->logResult("1/2-1/2");
}


void ChessController::declineDraw() {
    m_matchManager->declineDraw();
}


// Implementación del método que llamará QML
void ChessController::toggleAnalysisMode() {
    if (m_isAnalysisMode) {
        // SI ESTABA ENCENDIDO: Vamos a apagarlo y volver al presente

        // 1. Ejecutamos un bucle que avanza hacia el futuro hasta que ya no queden jugadas
        while (m_game.stepForwardAnalysis()) {
            m_logger->goForward(); // Avanzamos el texto
            m_currentTurn = (m_currentTurn == "white") ? "black" : "white"; // Sincronizamos el turno
        }

        // 2. Apagamos la variable de análisis
        m_isAnalysisMode = false;

        // 3. Limpiamos selecciones y actualizamos la interfaz gráfica UNA SOLA VEZ
        m_selectedRow = -1;
        m_selectedCol = -1;
        m_boardModel->clearSelectionsAndHighlights();
        m_boardModel->updateFromGame(m_game);

        // 4. Avisamos a QML que los botones deben cambiar
        emit analysisModeChanged();
        emit turnChanged();

        qDebug() << "Saliendo de Análisis: Tablero devuelto al presente automáticamente.";

    } else {
        // SI ESTABA APAGADO: Simplemente lo encendemos
        m_isAnalysisMode = true;
        emit analysisModeChanged();

        qDebug() << "Entrando al Modo Análisis (Modo Lectura).";
    }
}

void ChessController::stepBackward() {
    // 1. Bloqueo de seguridad: Solo funciona si el modo análisis está activo
    if (!m_isAnalysisMode) return;

    // 2. Intentamos retroceder en el backend lógico
    if (m_game.stepBackwardAnalysis()) {

        // 3. Retrocedemos el texto en el panel
        m_logger->goBackward();

        // 4. Cambiamos el turno visualmente para saber de quién es la jugada en este punto del pasado
        m_currentTurn = (m_currentTurn == "white") ? "black" : "white";

        // 5. Limpiamos cualquier clic "fantasma" que el usuario haya dejado a medias
        m_selectedRow = -1;
        m_selectedCol = -1;
        m_boardModel->clearSelectionsAndHighlights();

        // 6. Sincronizamos la matriz visual con el backend
        m_boardModel->updateFromGame(m_game);

        // 7. Notificamos a QML que el turno cambió
        emit turnChanged();

        qDebug() << "Análisis: Paso hacia atrás completado.";
    }
}

void ChessController::stepForward() {
    // 1. Bloqueo de seguridad: Solo funciona si el modo análisis está activo
    if (!m_isAnalysisMode) return;

    // 2. Intentamos avanzar hacia el futuro en el backend
    if (m_game.stepForwardAnalysis()) {

        // 3. Avanzamos el texto en el panel
        m_logger->goForward();

        // 4. Cambiamos el turno visualmente
        m_currentTurn = (m_currentTurn == "white") ? "black" : "white";

        // 5. Limpiamos selecciones
        m_selectedRow = -1;
        m_selectedCol = -1;
        m_boardModel->clearSelectionsAndHighlights();

        // 6. Sincronizamos la matriz visual
        m_boardModel->updateFromGame(m_game);

        // 7. Notificamos a QML
        emit turnChanged();

        qDebug() << "Análisis: Paso hacia adelante completado.";
    }
}


void ChessController::restartGame() {
    m_game = Game(new HumanPlayer(Color::WHITE), new HumanPlayer(Color::BLACK));
    m_currentTurn = "white";

    // Apaga el modo analisis
    m_isAnalysisMode = false;
    emit analysisModeChanged();

    // Limpiamos el estado de fin de juego
    m_matchManager->reset();

    // Limpiamos el historial de movimientos
    m_logger->clearLog();

    // Reiniciamos el contador de los relojs
    m_clock->start(10);

    m_boardModel->updateFromGame(m_game);
    emit turnChanged();
}


void ChessController::startLocalGame() {
    // 1. Instanciamos dos jugadores humanos
    m_game = Game(new HumanPlayer(Color::WHITE), new HumanPlayer(Color::BLACK));

    // 2. Reseteamos el estado visual y lógico (Misma lógica que en restartGame)
    m_currentTurn = "white";
    m_isAnalysisMode = false;
    emit analysisModeChanged();

    m_matchManager->reset();
    m_logger->clearLog();
    m_clock->start(10);

    m_boardModel->updateFromGame(m_game);
    emit turnChanged();

    qDebug() << "Partida Local (PvP) iniciada con éxito.";
}


void ChessController::startGameVsAI(int difficultyLevel) {
    // 1. TODO: Reemplazar el segundo HumanPlayer con tu IA cuando esté lista.
    // Debería quedar algo así:
    // m_game = Game(new HumanPlayer(Color::WHITE), new AIPlayer(Color::BLACK, difficultyLevel));

    // Por AHORA, instanciamos dos humanos para que el juego no haga crash al iniciar
    m_game = Game(new HumanPlayer(Color::WHITE), new HumanPlayer(Color::BLACK));

    // 2. Reseteamos el estado visual y lógico
    m_currentTurn = "white";
    m_isAnalysisMode = false;
    emit analysisModeChanged();

    m_matchManager->reset();
    m_logger->clearLog();
    m_clock->start(10);

    m_boardModel->updateFromGame(m_game);
    emit turnChanged();

    qDebug() << "Partida vs IA (PvE) iniciada. Nivel de dificultad:" << difficultyLevel;
    qDebug() << "AVISO: IA no implementada aún. Jugando modo Local temporalmente.";
}


void ChessController::checkGameOver() {
    if (m_game.isGameOver()) {
        QString reason = QString::fromStdString(m_game.getEndReason()); // "Jaque Mate" o "Rey Ahogado"
        QString winnerStr = QString::fromStdString(m_game.getWinner());
        QString winner = winnerStr == "Empate" ? "Empate mutuo" : "Ganador: " + winnerStr;

        // Le pasamos la información al Árbitro para que él declare el fin del juego
        m_matchManager->endMatch(reason, winner);
        if (reason != "Jaque Mate") {
            if (winnerStr == "Empate") {
                m_logger->logResult("1/2-1/2");
            } else if (winnerStr == "Blancas") {
                m_logger->logResult("1-0");
            } else if (winnerStr == "Negras") {
                m_logger->logResult("0-1");
            }
        }
    }
}
