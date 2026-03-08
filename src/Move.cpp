#include "../include/Move.hpp"
#include "../include/Piece.hpp" // Aquí si incluimos Piece.h completo si necesitamos usar sus metodos

// Implementacion del Constructor usando listas de inicializacion (mas rapido en C++)
Move::Move(Position f, Position t): from(f), to(t), capturedPiece(nullptr) {
}

// Implementacion de los Getters
Position Move::getFrom() const {
    return from;
}

Position Move::getTo() const {
    return to;
}

// Implementacion del metodo isCapture (Devuelve true si el puntero 'captured' no es nulo)
bool Move::isCapture() const {
    return capturedPiece != nullptr;
}

// Metodo para que Board le entregue la pieza capturada a Move
void Move::setCapturedPiece(std::unique_ptr<Piece> piece){
    capturedPiece = std::move(piece);
}

std::unique_ptr<Piece> Move::releaseCapturedPiece() {
    return std::move(capturedPiece); // El Move devuelve la memoria al tablero
}
