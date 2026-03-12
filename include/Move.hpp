#pragma once // Evita que el archivo se incluya múltiples veces en la compilación
#include <memory>
#include "Position.hpp"

// "Forward declaration" (Declaración anticipada) de Piece. 
// Le dice al compilador "Piece es una clase", sin necesidad de incluir todo Piece.h aquí.

class Piece;

class Move
{
    private:
        Position from;
        Position to;
        std::unique_ptr<Piece> capturedPiece;

    public:
        //Constructor
        Move(Position from, Position to);

        Position getFrom() const;
        Position getTo() const;

        // Prohibir copias explicitamente (Buena practica)
        Move(const Move&) = delete;
        Move& operator=(const Move&) = delete;

        // Permitir traslados (Move semantics)
        Move(Move&&) noexcept = default;
        Move& operator=(Move&&) noexcept = default;

        bool isCapture () const;

        void setCapturedPiece(std::unique_ptr<Piece> piece);

        std::unique_ptr<Piece> releaseCapturedPiece();

};
