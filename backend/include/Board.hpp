#pragma once

#include <memory>
#include "Position.hpp"
#include "Piece.hpp"
#include "Move.hpp"

class Board
{
    private:
        // Matriz 8x8 del tablero
        // Se usan punteros inteligentes para el manejo de la memoria
        // Se usa Piece para guardar las fichas y si esta vacia sera nulo

        std::unique_ptr<Piece> grid[8][8];
        int whiteMaterial;
        int blackMaterial;


    public:
        // Constructor
        Board();

        // Getters para que la IA los lea rapidamente
        int getWhiteMaterial() const;
        int getBlackMaterial() const;

        // Metodos de consulta
        bool isValidPosition (const Position& pos) const;
        bool isEmpty (const Position& pos) const;


        // Retorna un puntero "Observador" a la pieza
        Piece* getPieceAt (const Position& pos)const;


        // Ejecuta el movimiento y guarda la pieza capturada en 'm' (si la hay)
        void movePiece(Move& m);

        // Revierte el movimiento y devuelve la pieza capturada al tablero
        void undoPiece(Move& m);

        //Metodos de modificacion
        void placePiece(const Position& pos, std::unique_ptr<Piece> piece);

        //Inicializa el juego con las fichas en sus posiciones
        void setupInitialPosition();

};

