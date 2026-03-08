#include "../include/Board.hpp"
#include "../include/Pawn.hpp"
#include "../include/Rook.hpp"
#include "../include/Knight.hpp"
#include "../include/Bishop.hpp"
#include "../include/Queen.hpp"
#include "../include/King.hpp"

Board::Board(){
    // Con esto instanciamos el arreglo grid[8][8] y todos los unique_ptr estan en null 
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
        m.setCapturedPiece(std::move(grid[to.getRow()][to.getCol()]));
    }

    // Movemos la pieza de from a to 
    grid[to.getRow()][to.getCol()] = std::move(grid[from.getRow()][from.getCol()]);
}


void Board::undoPiece(Move& m){
    Position from = m.getFrom();
    Position to = m.getTo();

    //Regrresamos la pieza a su origen 
    grid[from.getRow()][from.getCol()] = std::move(grid[to.getRow()][to.getCol()]);

    // ReColocamos la pieza capturada en la casilla correspondiente 
    if (m.isCapture()){
        grid[to.getRow()][to.getCol()] = m.releaseCapturedPiece();
    }
}


// Coloca una nueva pieza para coronar un peon o para iniciar el juego
void Board::placePiece(const Position& pos, std::unique_ptr<Piece> piece){
    if(isValidPosition(pos)){
        grid[pos.getRow()][pos.getCol()] = std::move(piece);
    }
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
