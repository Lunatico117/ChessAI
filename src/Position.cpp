#include "../include/Position.hpp"

// Implementacion del constructor por defecto (inicializa en 0,0)
Position :: Position (): row (0), col (0){
}

// Implementacion del constructor parametrizado usando listas de inicializacion
Position :: Position(int r, int c) : row (r), col (c){
}

// Implementacion de los Getters
int Position::getCol()const{
    return col;
}

int Position::getRow()const{
    return row;
}

// Implementacion del metodo EQUALS 
bool Position::equals(const Position& p) const {
    return (this->row == p.row && this->col == p.col );
}

bool Position::operator==(const Position& p) const {
    return this ->equals (p);
}
