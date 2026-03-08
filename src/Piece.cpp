#include "../include/Piece.hpp"

// Implementaicon del cosntructor usando la lista de inicializacion
Piece::Piece(Color c): color(c){ 
}

// Implementacion del Getter 
Color Piece::getColor()const {
    return color;
}

// El resto de implementaciones de posible movimiento se hacen directamente en las subclases
// Ya que Piece es una clase abstracta
