#include "../include/ChessController.hpp"
#include "../backend/include/PlayerFactory.hpp"
#include <QDebug>

ChessController::ChessController(QObject *parent)
    : QObject(parent),
    // Usamos la fabrica para crear el juego base (Dummy) al iniciar la app
    m_game(PlayerFactory::createPlayer("Human", Color::WHITE), PlayerFactory::createPlayer("Human", Color::BLACK)),    m_boardModel(new BoardModel(this)),
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

    if (m_isVsAI && m_currentTurn == "black") {
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
                // --- MOVIMIENTO NORMAL HUMANO ---
                MoveResult res = m_game.processHumanMove(from, to);

                if (res.success) {
                    bool isMate = (m_game.isGameOver() && m_game.getEndReason() == "Jaque Mate");
                    bool isCheck = false;

                    if (!isMate) {
                        Color opponentColor = (m_currentTurn == "white") ? Color::BLACK : Color::WHITE;
                        isCheck = m_game.isInCheck(opponentColor);
                    }

                    // El Logger usa el DTO directo
                    m_logger->logMove(res.record, isCheck, isMate);

                    m_boardModel->clearSelectionsAndHighlights();
                    m_boardModel->updateFromGame(m_game);
                    m_currentTurn = (m_currentTurn == "white") ? "black" : "white";
                    m_clock->switchTurn(m_currentTurn);
                    emit turnChanged();
                    checkGameOver();

                    if (m_isVsAI && m_currentTurn == "black" && !m_game.isGameOver()) {
                        processAITurn();
                    }
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

    if (m_isVsAI && m_currentTurn == "black") {
        return;
    }

    PromotionType promoType = PromotionType::QUEEN;
    if (pieceType == "rook") promoType = PromotionType::ROOK;
    else if (pieceType == "bishop") promoType = PromotionType::BISHOP;
    else if (pieceType == "knight") promoType = PromotionType::KNIGHT;

    // --- MOVIMIENTO DE CORONACIÓN HUMANO ---
    MoveResult res = m_game.processHumanMove(
        Position(m_pendingFromRow, m_pendingFromCol),
        Position(m_pendingToRow, m_pendingToCol),
        promoType
        );

    if (res.success) {
        bool isMate = (m_game.isGameOver() && m_game.getEndReason() == "Jaque Mate");
        bool isCheck = false;

        if (!isMate) {
            Color opponentColor = (m_currentTurn == "white") ? Color::BLACK : Color::WHITE;
            isCheck = m_game.isInCheck(opponentColor);
        }

        m_logger->logMove(res.record, isCheck, isMate);

        m_boardModel->clearSelectionsAndHighlights();
        m_boardModel->updateFromGame(m_game);
        m_currentTurn = (m_currentTurn == "white") ? "black" : "white";
        m_clock->switchTurn(m_currentTurn);
        emit turnChanged();
        checkGameOver();

        if (m_isVsAI && m_currentTurn == "black" && !m_game.isGameOver()) {
            processAITurn();
        }
    }

    m_pendingFromRow = -1;
    m_pendingFromCol = -1;
    m_pendingToRow = -1;
    m_pendingToCol = -1;
}

void ChessController::processAITurn() {
    qDebug() << "processAITurn() convocado...";

    if (!m_game.isGameOver() && m_currentTurn == "black") {
        QTimer::singleShot(500, this, [this]() {

            // --- TURNO DE LA IA (Simétrico al humano) ---
            MoveResult res = m_game.executeAITurn();

            if (res.success) {
                bool isMate = (m_game.isGameOver() && m_game.getEndReason() == "Jaque Mate");
                bool isCheck = false;
                if (!isMate) {
                    isCheck = m_game.isInCheck(Color::WHITE);
                }

                m_logger->logMove(res.record, isCheck, isMate);

                m_boardModel->updateFromGame(m_game);
                m_currentTurn = "white";
                m_clock->switchTurn(m_currentTurn);

                emit turnChanged();
                checkGameOver();
            }
        });
    }
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
    Player* p1 = PlayerFactory::createPlayer("Human", Color::WHITE);
    Player* p2 = nullptr;

    if (m_isVsAI) {
        p2 = PlayerFactory::createPlayer("AI", Color::BLACK, m_currentDifficulty);
    } else {
        p2 = PlayerFactory::createPlayer("Human", Color::BLACK);
    }

    m_game = Game(p1, p2);

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
    m_isVsAI = false;
    Player* p1 = PlayerFactory::createPlayer("Human", Color::WHITE);
    Player* p2 = PlayerFactory::createPlayer("Human", Color::BLACK);

    m_game = Game(p1, p2);

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
    m_isVsAI = true;
    m_currentDifficulty = difficultyLevel;

    Player* p1 = PlayerFactory::createPlayer("Human", Color::WHITE);
    Player* p2 = PlayerFactory::createPlayer("AI", Color::BLACK, difficultyLevel);

    m_game = Game(p1, p2);

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


