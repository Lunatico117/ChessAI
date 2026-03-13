#include "../include/Player.hpp"

// Implementacion del constructor usando la lista de inicializacion
Player::Player(Color c) : color(c) {
}

// Implementacion del getter
Color Player::getColor() const {
    return color;
}

// getMove() no se implementa aqui porque es "= 0" (puramente virtual)
