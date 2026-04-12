#include "../include/Board.hpp"
#include "../include/Pawn.hpp"
#include "../include/Rook.hpp"
#include "../include/Knight.hpp"
#include "../include/Bishop.hpp"
#include "../include/Queen.hpp"
#include "../include/King.hpp"
#include "../include/ai/PieceSquareTables.hpp"

Board::Board() : whiteMaterial(0), blackMaterial(0), whitePositionalScore(0), blackPositionalScore(0), whiteBishops(0), blackBishops(0){
    for (int i = 0; i < 8; ++i) {
        whitePawnsOnFile[i] = 0;
        blackPawnsOnFile[i] = 0;
    }

    // Con esto instanciamos el arreglo grid[8][8] y todos los unique_ptr estan en null
}



int Board::getWhiteMaterial() const
    { return whiteMaterial;
}

int Board::getBlackMaterial() const {
    return blackMaterial;
}

int Board::getWhitePositionalScore() const {
    return whitePositionalScore;
}

int Board::getBlackPositionalScore() const {
    return blackPositionalScore;
}

// Verifica si la posicion este dentro del tablero 
bool Board::isValidPosition(const Position& pos) const {
    // Revisa que este dentro de los limites del tablero
    return pos.getRow () >= 0 && pos.getRow() < 8 && pos.getCol () >= 0 && pos.getCol () < 8;
}


// Verifica si la casilla esta vacia
bool Board::isEmpty(const Position& pos) const{
    if(!isValidPosition(pos)) return false;
    // Es vacia si el puntero es nulo

    return grid[pos.getRow()][pos.getCol()] == nullptr;
}


// Tiene un vistazo a los atributos de la pieza
Piece* Board::getPieceAt(const Position& pos)const {
    if(!isValidPosition(pos)) return nullptr; // Puede ser false en ves de nullptr

    return grid[pos.getRow()][pos.getCol()].get();
}


// Mueve la pieza
void Board::movePiece(Move& m){
    Position from = m.getFrom();
    Position to = m.getTo();

    // Verificacion extra
    if (!isValidPosition(from) || !isValidPosition(to)) return;

    // Si hay una pieza enemiga en la casilla el objeto Move la captura para que no se destruya de inmediato 
    if (grid[to.getRow()][to.getCol()] != nullptr){
        // ¡IMPORTANTE! Leemos la pieza ANTES de hacer el std::move
        Piece* capturedPiece = grid[to.getRow()][to.getCol()].get();

        // Leemos el score posicional de la pieza que va a ser capturada
        int capPosScore = PST::getPositionalBonus(capturedPiece->getType(), capturedPiece->getColor(), to);

        // ACTUALIZACIÓN INCREMENTAL: Restar el material al jugador que pierde la pieza
        if (capturedPiece->getColor() == Color::WHITE) {
            whiteMaterial -= getPieceValue(capturedPiece->getType());
            whitePositionalScore -= capPosScore; // NUEVO: Restamos el score posicional

            // Restar contadores si capturamos un Alfil o un Peon enemigo
            if (capturedPiece->getType() == PieceType::BISHOP) whiteBishops--;
            else if (capturedPiece->getType() == PieceType::PAWN) whitePawnsOnFile[to.getCol()]--;

        }
        else {
            blackMaterial -= getPieceValue(capturedPiece->getType());
            blackPositionalScore -= capPosScore; // NUEVO: Restamos el score posicional

            // Restar contadores si capturamos un Alfil o un Peon enemigo
            if (capturedPiece->getType() == PieceType::BISHOP) blackBishops--;
            else if (capturedPiece->getType() == PieceType::PAWN) blackPawnsOnFile[to.getCol()]--;
        }

        // Ahora sí, el objeto Move la captura
        m.setCapturedPiece(std::move(grid[to.getRow()][to.getCol()]));
    }


    // Comportamiento segun el tipo de movimiento
    switch (m.getType()){
        case MoveType::NORMAL:{

            // Calculamos la diferencia posicional antes de mover
            Piece* moving = grid[from.getRow()][from.getCol()].get();
            int posDiff = PST::getPositionalBonus(moving->getType(), moving->getColor(), to) - PST::getPositionalBonus(moving->getType(), moving->getColor(), from);
            if (moving->getColor() == Color::WHITE) whitePositionalScore += posDiff;
            else blackPositionalScore += posDiff;

            // Si un peon se mueve de columna actualizamos los contadores de columnas
            if (moving->getType() == PieceType::PAWN && from.getCol() != to.getCol()) {
                if (moving->getColor() == Color::WHITE) {
                    whitePawnsOnFile[from.getCol()]--;
                    whitePawnsOnFile[to.getCol()]++;
                } else {
                    blackPawnsOnFile[from.getCol()]--;
                    blackPawnsOnFile[to.getCol()]++;
                }
            }

            // Movemos la pieza de from a to
            grid[to.getRow()][to.getCol()] = std::move(grid[from.getRow()][from.getCol()]);
            break;
        }

        case MoveType::PROMOTION:{
            // Obtenemos el color de la pieza del peon
            Color pieceColor = grid[from.getRow()][from.getCol()] -> getColor();
            int pawnValue = getPieceValue(grid[from.getRow()][from.getCol()]->getType());

            // Obtenemos el valor posicional del peon en la casilla de origen
            int pawnPosScore = PST::getPositionalBonus(PieceType::PAWN, pieceColor, from);

            // ACTUALIZACION INCREMENTAL: El peon va a desaparecer, restamos su valor
            if (pieceColor == Color::WHITE) {
                whiteMaterial -= pawnValue;
                whitePositionalScore -= pawnPosScore; // Restamos posicional del peon
                whitePawnsOnFile[from.getCol()]--; // El peon desaparece de esta columna
            } else {
                blackMaterial -= pawnValue;
                blackPositionalScore -= pawnPosScore; // Restamos posicional del peon
                blackPawnsOnFile[from.getCol()]--; // El peon desaparece de esta columna
            }
            // Colocamos la nueva pieza pero dependiendo del tipo de coronacion

            switch (m.getPromotionType()) {

                case PromotionType::QUEEN: grid[to.getRow()][to.getCol()] = std::make_unique<Queen>(pieceColor);
                    break;
                case PromotionType::ROOK: grid[to.getRow()][to.getCol()] = std::make_unique<Rook>(pieceColor);
                    break;
                case PromotionType::BISHOP: grid[to.getRow()][to.getCol()] = std::make_unique<Bishop>(pieceColor);
                    if (pieceColor == Color::WHITE) whiteBishops++; else blackBishops++; // Suma a la cantidad de alfiles del jugador
                    break;
                case PromotionType::KNIGHT: grid[to.getRow()][to.getCol()] = std::make_unique<Knight>(pieceColor);
                    break;

                default:
                    break;
                }
                // ACTUALIZACION INCREMENTAL: Sumamos el valor de la nueva pieza
                int promotedValue = getPieceValue(grid[from.getRow()][from.getCol()]->getType());

                // Sumamos el valor posicional de la NUEVA pieza en la casilla de destino ('to')
                int promotedPosScore = PST::getPositionalBonus(grid[to.getRow()][to.getCol()]->getType(), pieceColor, to);

                if (pieceColor == Color::WHITE) {
                    whiteMaterial += promotedValue;
                    whitePositionalScore += promotedPosScore;
                } else {
                    blackMaterial += promotedValue;
                    blackPositionalScore += promotedPosScore;
                }
            // Vaciamos la casilla de origen (el peon desaparece)
            grid[from.getRow()][from.getCol()] = nullptr;
            break;
        }

        case MoveType::EN_PASSANT: {
            // Calculamos la diferencia posicional del peOn que ataca antes de moverlo
            Piece* moving = grid[from.getRow()][from.getCol()].get();
            int posDiff = PST::getPositionalBonus(moving->getType(), moving->getColor(), to) - PST::getPositionalBonus(moving->getType(), moving->getColor(), from);
            if (moving->getColor() == Color::WHITE) {
                whitePositionalScore += posDiff;
                // El peon atacante cambia de columna
                whitePawnsOnFile[from.getCol()]--;
                whitePawnsOnFile[to.getCol()]++;
            } else {
                blackPositionalScore += posDiff;
                // El peon atacante cambia de columna
                blackPawnsOnFile[from.getCol()]--;
                blackPawnsOnFile[to.getCol()]++;
            }

            // Movemos el peon que va a capturar a la casilla en diagonal
            grid[to.getRow()][to.getCol()] = std::move(grid[from.getRow()][from.getCol()]);

            // El peon capturado NO estaba en 'to', estaba al lado de 'from'
            Piece* capturedPawn = grid[from.getRow()][to.getCol()].get();

            if (capturedPawn != nullptr) {

                int capPosScore = PST::getPositionalBonus(capturedPawn->getType(), capturedPawn->getColor(), Position(from.getRow(), to.getCol()));
                // ACTUALIZACION INCREMENTAL
                if (capturedPawn->getColor() == Color::WHITE){
                    whiteMaterial -= getPieceValue(capturedPawn->getType());
                    whitePositionalScore -= capPosScore;
                    whitePawnsOnFile[to.getCol()]--; // El peon enemigo capturado desaparece de la columna 'to'

                }
                else {
                    blackMaterial -= getPieceValue(capturedPawn->getType());
                    blackPositionalScore -= capPosScore;
                    blackPawnsOnFile[to.getCol()]--; // El peon enemigo capturado desaparece de la columna 'to'
                }
            }
            // Capturamos la pieza que estaba al lado, es decir en la misma fila pero en la columna de la diagonal
            m.setCapturedPiece(std::move(grid[from.getRow()][to.getCol()]));

            // Vaciamos la casilla
            grid[from.getRow()][to.getCol()] = nullptr;
            break;

        }


        case MoveType::CASTLING: {

            // Diferencia posicional del Rey
            Piece* king = grid[from.getRow()][from.getCol()].get();
            int kingDiff = PST::getPositionalBonus(king->getType(), king->getColor(), to) - PST::getPositionalBonus(king->getType(), king->getColor(), from);
            if (king->getColor() == Color::WHITE) whitePositionalScore += kingDiff;
            else blackPositionalScore += kingDiff;

            // Movemos al rey a su lugar en el enroque
            grid[to.getRow()][to.getCol()] = std::move(grid[from.getRow()][from.getCol()]);

            // Movemos a la torre a su lugar en el enroque
            int row = to.getRow();
            if (to.getCol() == 6) { // Enroque Corto (El Rey fue a la columna 6)

                // Diferencia posicional de la Torre (De columna 7 a 5)
                Piece* rook = grid[row][7].get();
                int rookDiff = PST::getPositionalBonus(rook->getType(), rook->getColor(), Position(row, 5)) - PST::getPositionalBonus(rook->getType(), rook->getColor(), Position(row, 7));
                if (rook->getColor() == Color::WHITE) whitePositionalScore += rookDiff;
                else blackPositionalScore += rookDiff;

                grid[row][5] = std::move(grid[row][7]); // La Torre va de la col 7 a la 5
            }

            else if (to.getCol() == 2) { // Enroque Largo (El Rey fue a la columna 2)

                // Diferencia posicional de la Torre (De columna 0 a 3)
                Piece* rook = grid[row][0].get();
                int rookDiff = PST::getPositionalBonus(rook->getType(), rook->getColor(), Position(row, 3)) - PST::getPositionalBonus(rook->getType(), rook->getColor(), Position(row, 0));
                if (rook->getColor() == Color::WHITE) whitePositionalScore += rookDiff;
                else blackPositionalScore += rookDiff;

                grid[row][3] = std::move(grid[row][0]); // La Torre va de la col 0 a la 3
            }
            break;
        }
    }
}


// Revierte el movimiento
void Board::undoPiece(Move& m){
    Position from = m.getFrom();
    Position to = m.getTo();

    switch (m.getType()){
    case MoveType::NORMAL:{

        // Invertimos la diferencia posicional antes de devolverla
        Piece* moving = grid[to.getRow()][to.getCol()].get();
        int posDiff = PST::getPositionalBonus(moving->getType(), moving->getColor(), from) - PST::getPositionalBonus(moving->getType(), moving->getColor(), to);
        if (moving->getColor() == Color::WHITE) whitePositionalScore += posDiff;
        else blackPositionalScore += posDiff;

        if (moving->getType() == PieceType::PAWN && from.getCol() != to.getCol()) {
            if (moving->getColor() == Color::WHITE) {
                whitePawnsOnFile[from.getCol()]++;
                whitePawnsOnFile[to.getCol()]--;
            } else {
                blackPawnsOnFile[from.getCol()]++;
                blackPawnsOnFile[to.getCol()]--;
            }
        }

        // Movemos la pieza de from a to
        grid[from.getRow()][from.getCol()] = std::move(grid[to.getRow()][to.getCol()]);
        break;
    }

    case MoveType::PROMOTION:{
        // Obtenemos la pieza antes de destruirla para restar su valor
        Piece* promotedPiece = grid[to.getRow()][to.getCol()].get();
        // Obtenemos el color de la pieza del peon
        Color pieceColor = grid[to.getRow()][to.getCol()] -> getColor();

        int promotedValue = getPieceValue(promotedPiece->getType());

        // Valor posicional de la pieza coronada a restar
        int promotedPosScore = PST::getPositionalBonus(promotedPiece->getType(), pieceColor, to);

        // ACTUALIZACION INCREMENTAL: Restamos la pieza que va a desaparecer
        if (pieceColor == Color::WHITE) {
            whiteMaterial -= promotedValue;
            whitePositionalScore -= promotedPosScore;
            if (promotedPiece->getType() == PieceType::BISHOP) whiteBishops--;
        } else {
            blackMaterial -= promotedValue;
            blackPositionalScore -= promotedPosScore;
            if (promotedPiece->getType() == PieceType::BISHOP) blackBishops--;
        }

        // Colocamos la nueva pieza pero dependiendo del tipo de coronacion
        grid[from.getRow()][from.getCol()] = std::make_unique<Pawn>(pieceColor);

        // Al revivir el peon, debemos sumar su valor de vuelta al tablero
        int pawnValue = getPieceValue(PieceType::PAWN);

        // Valor posicional del peon que revive en 'from'
        int pawnPosScore = PST::getPositionalBonus(PieceType::PAWN, pieceColor, from);

        if (pieceColor == Color::WHITE) {
            whiteMaterial += pawnValue;
            whitePositionalScore += pawnPosScore;
            whitePawnsOnFile[from.getCol()]++; // El peon original reaparece en su columna
        } else {
            blackMaterial += pawnValue;
            blackPositionalScore += pawnPosScore;
            blackPawnsOnFile[from.getCol()]++; // El peon original reaparece en su columna
        }

        // Vaciamos la casilla de origen (el peon desaparece)
        grid[to.getRow()][to.getCol()] = nullptr;
        break;
    }

    case MoveType::EN_PASSANT: {
        // Invertimos la diferencia posicional del peon atacante
        Piece* moving = grid[to.getRow()][to.getCol()].get();
        int posDiff = PST::getPositionalBonus(moving->getType(), moving->getColor(), from) - PST::getPositionalBonus(moving->getType(), moving->getColor(), to);

        if (moving->getColor() == Color::WHITE) {
            whitePositionalScore += posDiff;
            // Revertimos el conteo en las columnas
            whitePawnsOnFile[from.getCol()]++;
            whitePawnsOnFile[to.getCol()]--;
        } else {
            blackPositionalScore += posDiff;
            // Revertimos el conteo en las columnas
            blackPawnsOnFile[from.getCol()]++;
            blackPawnsOnFile[to.getCol()]--;
        }

        // Devolvemos el peon a su posicion original
        grid[from.getRow()][from.getCol()] = std::move(grid[to.getRow()][to.getCol()]);
        break;
    }

    case MoveType::CASTLING: {

        // Invertimos la diferencia posicional del Rey
        Piece* king = grid[to.getRow()][to.getCol()].get();
        int kingDiff = PST::getPositionalBonus(king->getType(), king->getColor(), from) - PST::getPositionalBonus(king->getType(), king->getColor(), to);
        if (king->getColor() == Color::WHITE) whitePositionalScore += kingDiff;
        else blackPositionalScore += kingDiff;

        // 1. Devolvemos el Rey a su posicion original
        grid[from.getRow()][from.getCol()] = std::move(grid[to.getRow()][to.getCol()]);

        // 2. Devolvemos la Torre a su posicion original
        int row = to.getRow();
        if (to.getCol() == 6) { // Era Enroque Corto
            // Invertimos la diferencia posicional de la Torre (De 5 vuelve a 7)
            Piece* rook = grid[row][5].get();
            int rookDiff = PST::getPositionalBonus(rook->getType(), rook->getColor(), Position(row, 7)) - PST::getPositionalBonus(rook->getType(), rook->getColor(), Position(row, 5));
            if (rook->getColor() == Color::WHITE) whitePositionalScore += rookDiff;
            else blackPositionalScore += rookDiff;

            grid[row][7] = std::move(grid[row][5]); // Torre vuelve a la col 7
        }
        else if (to.getCol() == 2) { // Era Enroque Largo
            // Invertimos la diferencia posicional de la Torre (De 3 vuelve a 0)
            Piece* rook = grid[row][3].get();
            int rookDiff = PST::getPositionalBonus(rook->getType(), rook->getColor(), Position(row, 0)) - PST::getPositionalBonus(rook->getType(), rook->getColor(), Position(row, 3));
            if (rook->getColor() == Color::WHITE) whitePositionalScore += rookDiff;
            else blackPositionalScore += rookDiff;

            grid[row][0] = std::move(grid[row][3]); // Torre vuelve a la col 0
        }
        break;
    }

    }

    // ReColocamos la pieza capturada en la casilla correspondiente 
    if (m.isCapture()){
        std::unique_ptr<Piece> revivedPiece = m.releaseCapturedPiece();

        // Evaluamos la posicion exacta donde la pieza revive (es diferente en En Passant)
        Position revivePos = (m.getType() == MoveType::EN_PASSANT) ? Position(from.getRow(), to.getCol()) : to;
        int capPosScore = PST::getPositionalBonus(revivedPiece->getType(), revivedPiece->getColor(), revivePos);

        if (revivedPiece->getColor() == Color::WHITE) {
            whiteMaterial += getPieceValue(revivedPiece->getType());
            whitePositionalScore += capPosScore; // Sumamos el score posicional de vuelta

            // NUEVO: Volvemos a sumar la pieza al contador si era Alfil o Peon
            if (revivedPiece->getType() == PieceType::BISHOP) {
                whiteBishops++;
            }
            else if (revivedPiece->getType() == PieceType::PAWN) {
                whitePawnsOnFile[revivePos.getCol()]++;
            }

        }
        else {
            blackMaterial += getPieceValue(revivedPiece->getType());
            blackPositionalScore += capPosScore; // Sumamos el score posicional de vuelta
            // Volvemos a sumar la pieza al contador si era Alfil o Peon
            if (revivedPiece->getType() == PieceType::BISHOP) {
                blackBishops++;
            }
            else if (revivedPiece->getType() == PieceType::PAWN){
                blackPawnsOnFile[revivePos.getCol()]++;
            }

        }

        // La re-colocamos en el tablero
        if (m.getType() == MoveType::EN_PASSANT) {
            // Se restaura en otra casilla distinta a la que llego
            grid[from.getRow()][to.getCol()] = std::move(revivedPiece);
        } else {
            // Captura normal o promocion
            grid[to.getRow()][to.getCol()] = std::move(revivedPiece);
        }
    }
}



// Coloca una nueva pieza para coronar un peon o para iniciar el juego
void Board::placePiece(const Position& pos, std::unique_ptr<Piece> piece) {
    if (piece != nullptr) {
        // Obtenemos el bonus posicional inicial
        int posScore = PST::getPositionalBonus(piece->getType(), piece->getColor(), pos);

        // ACTUALIZACION INCREMENTAL: Sumar material
        if (piece->getColor() == Color::WHITE) {
            whiteMaterial += getPieceValue(piece->getType());
            whitePositionalScore += posScore;
            // Inicializar contadores al colocar piezas en el tablero
            if (piece->getType() == PieceType::BISHOP) {
                whiteBishops++;
            }
            else if (piece->getType() == PieceType::PAWN) {
                whitePawnsOnFile[pos.getCol()]++;
            }
        }
        else {
            blackMaterial += getPieceValue(piece->getType());
            blackPositionalScore += posScore;
            // Inicializar contadores al colocar piezas en el tablero
            if (piece->getType() == PieceType::BISHOP) {
                blackBishops++;
            }
            else if (piece->getType() == PieceType::PAWN) {
                blackPawnsOnFile[pos.getCol()]++;
            }
        }
    }
    // Tu código original para poner la pieza en el grid
    grid[pos.getRow()][pos.getCol()] = std::move(piece);
}


// Cada pieza en su correspondiente lugar al iniciar
void Board::setupInitialPosition() {
    // Peones (Negras en fila 1, Blancas en fila 6)
    for (int col = 0; col < 8; ++col) {
        placePiece(Position(1, col), std::make_unique<Pawn>(Color::BLACK));
        placePiece(Position(6, col), std::make_unique<Pawn>(Color::WHITE));
    }

    // Torres (Columnas 0 y 7)
    placePiece(Position(0, 0), std::make_unique<Rook>(Color::BLACK));
    placePiece(Position(0, 7), std::make_unique<Rook>(Color::BLACK));
    placePiece(Position(7, 0), std::make_unique<Rook>(Color::WHITE));
    placePiece(Position(7, 7), std::make_unique<Rook>(Color::WHITE));

    // Caballos (Columnas 1 y 6)
    placePiece(Position(0, 1), std::make_unique<Knight>(Color::BLACK));
    placePiece(Position(0, 6), std::make_unique<Knight>(Color::BLACK));
    placePiece(Position(7, 1), std::make_unique<Knight>(Color::WHITE));
    placePiece(Position(7, 6), std::make_unique<Knight>(Color::WHITE));

    // Alfiles (Columnas 2 y 5)
    placePiece(Position(0, 2), std::make_unique<Bishop>(Color::BLACK));
    placePiece(Position(0, 5), std::make_unique<Bishop>(Color::BLACK));
    placePiece(Position(7, 2), std::make_unique<Bishop>(Color::WHITE));
    placePiece(Position(7, 5), std::make_unique<Bishop>(Color::WHITE));

    // Reinas (Columna 3)
    placePiece(Position(0, 3), std::make_unique<Queen>(Color::BLACK));
    placePiece(Position(7, 3), std::make_unique<Queen>(Color::WHITE));

    // Reyes (Columna 4)
    placePiece(Position(0, 4), std::make_unique<King>(Color::BLACK));
    placePiece(Position(7, 4), std::make_unique<King>(Color::WHITE));
}
