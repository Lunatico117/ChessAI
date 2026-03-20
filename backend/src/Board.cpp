#include "../include/Board.hpp"
#include "../include/Pawn.hpp"
#include "../include/Rook.hpp"
#include "../include/Knight.hpp"
#include "../include/Bishop.hpp"
#include "../include/Queen.hpp"
#include "../include/King.hpp"

Board::Board() : whiteMaterial(0), blackMaterial(0){
    // Con esto instanciamos el arreglo grid[8][8] y todos los unique_ptr estan en null 
    }



int Board::getWhiteMaterial() const
    { return whiteMaterial;
}

int Board::getBlackMaterial() const {
    return blackMaterial;
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

        // ACTUALIZACIÓN INCREMENTAL: Restar el material al jugador que pierde la pieza
        if (capturedPiece->getColor() == Color::WHITE) {
            whiteMaterial -= capturedPiece->getValue();
        } else {
            blackMaterial -= capturedPiece->getValue();
        }

        // Ahora sí, el objeto Move la captura
        m.setCapturedPiece(std::move(grid[to.getRow()][to.getCol()]));
    }


    // Comportamiento segun el tipo de movimiento
    switch (m.getType()){
        case MoveType::NORMAL:{
            // Movemos la pieza de from a to
            grid[to.getRow()][to.getCol()] = std::move(grid[from.getRow()][from.getCol()]);
            break;
        }

        case MoveType::PROMOTION:{
            // Obtenemos el color de la pieza del peon
            Color pieceColor = grid[from.getRow()][from.getCol()] -> getColor();
            int pawnValue = grid[from.getRow()][from.getCol()]->getValue(); // Será 100

            // ACTUALIZACIÓN INCREMENTAL: El peón va a desaparecer, restamos su valor
            if (pieceColor == Color::WHITE) whiteMaterial -= pawnValue;
            else blackMaterial -= pawnValue;
            // Colocamos la nueva pieza pero dependiendo del tipo de coronacion

            switch (m.getPromotionType()) {

                case PromotionType::QUEEN: grid[to.getRow()][to.getCol()] = std::make_unique<Queen>(pieceColor);
                    break;
                case PromotionType::ROOK: grid[to.getRow()][to.getCol()] = std::make_unique<Rook>(pieceColor);
                    break;
                case PromotionType::BISHOP: grid[to.getRow()][to.getCol()] = std::make_unique<Bishop>(pieceColor);
                    break;
                case PromotionType::KNIGHT: grid[to.getRow()][to.getCol()] = std::make_unique<Knight>(pieceColor);
                    break;

                default:
                    break;
                }
                // ACTUALIZACIÓN INCREMENTAL: Sumamos el valor de la nueva súper pieza (ej. +900)
                int promotedValue = grid[to.getRow()][to.getCol()]->getValue();
                if (pieceColor == Color::WHITE) whiteMaterial += promotedValue;
                else blackMaterial += promotedValue;

            // Vaciamos la casilla de origen (el peon desaparece)
            grid[from.getRow()][from.getCol()] = nullptr;
            break;
        }

        case MoveType::EN_PASSANT: {
            // Movemos el peon que va a capturar a la casilla en diagonal
            grid[to.getRow()][to.getCol()] = std::move(grid[from.getRow()][from.getCol()]);

            // El peón capturado NO estaba en 'to', estaba al lado de 'from'
            Piece* capturedPawn = grid[from.getRow()][to.getCol()].get();

            if (capturedPawn != nullptr) {
                // ACTUALIZACIÓN INCREMENTAL
                if (capturedPawn->getColor() == Color::WHITE) whiteMaterial -= capturedPawn->getValue();
                else blackMaterial -= capturedPawn->getValue();
            }

            // Capturamos la pieza que estaba al lado, es decir en la misma fila pero en la columna de la diagonal
            m.setCapturedPiece(std::move(grid[from.getRow()][to.getCol()]));

            // Vaciamos la casilla
            grid[from.getRow()][to.getCol()] = nullptr;
            break;
        }

        case MoveType::CASTLING: {
            // Movemos al rey a su lugar en el enroque
            grid[to.getRow()][to.getCol()] = std::move(grid[from.getRow()][from.getCol()]);

            // Movemos a la torre a su lugar en el enroque
            int row = to.getRow();
            if (to.getCol() == 6) { // Enroque Corto (El Rey fue a la columna 6)
                grid[row][5] = std::move(grid[row][7]); // La Torre va de la col 7 a la 5
            }
            else if (to.getCol() == 2) { // Enroque Largo (El Rey fue a la columna 2)
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
        // Movemos la pieza de from a to
        grid[from.getRow()][from.getCol()] = std::move(grid[to.getRow()][to.getCol()]);
        break;
    }

    case MoveType::PROMOTION:{
        // Obtenemos la pieza antes de destruirla para restar su valor
        Piece* promotedPiece = grid[to.getRow()][to.getCol()].get();
        // Obtenemos el color de la pieza del peon
        Color pieceColor = grid[to.getRow()][to.getCol()] -> getColor();

        int promotedValue = promotedPiece->getValue(); // Ej: 900 (Reina)

        // ACTUALIZACIÓN INCREMENTAL: Restamos la súper pieza que va a desaparecer
        if (pieceColor == Color::WHITE) whiteMaterial -= promotedValue;
        else blackMaterial -= promotedValue;

        // Colocamos la nueva pieza pero dependiendo del tipo de coronacion
        grid[from.getRow()][from.getCol()] = std::make_unique<Pawn>(pieceColor);

        // Vaciamos la casilla de origen (el peon desaparece)
        grid[to.getRow()][to.getCol()] = nullptr;
        break;
    }

    case MoveType::EN_PASSANT: {
        // Devolvemos el peon a su posicion original
        grid[from.getRow()][from.getCol()] = std::move(grid[to.getRow()][to.getCol()]);
        break;
    }

    case MoveType::CASTLING: {
        // 1. Devolvemos el Rey a su posicion original
        grid[from.getRow()][from.getCol()] = std::move(grid[to.getRow()][to.getCol()]);

        // 2. Devolvemos la Torre a su posicion original
        int row = to.getRow();
        if (to.getCol() == 6) { // Era Enroque Corto
            grid[row][7] = std::move(grid[row][5]); // Torre vuelve a la col 7
        }
        else if (to.getCol() == 2) { // Era Enroque Largo
            grid[row][0] = std::move(grid[row][3]); // Torre vuelve a la col 0
        }
        break;
    }

    }

    // ReColocamos la pieza capturada en la casilla correspondiente 
    if (m.isCapture()){
        std::unique_ptr<Piece> revivedPiece = m.releaseCapturedPiece();

        if (revivedPiece->getColor() == Color::WHITE) {
            whiteMaterial += revivedPiece->getValue();
        } else {
            blackMaterial += revivedPiece->getValue();
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
        // ACTUALIZACIÓN INCREMENTAL: Sumar material
        if (piece->getColor() == Color::WHITE) {
            whiteMaterial += piece->getValue();
        } else {
            blackMaterial += piece->getValue();
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
