#include "../include/Piece.hpp"

// 1. El constructor ahora recibe y asigna ambos valores
Piece::Piece(Color c, PieceType t) : color(c), type(t) {
}

// 2. Getter del color (se queda igual)
Color Piece::getColor() const {
    return color;
}

// 3. Getter del tipo (¡Nuevo!)
PieceType Piece::getType() const {
    return type;
}
