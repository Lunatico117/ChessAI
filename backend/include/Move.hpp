#pragma once // Evita que el archivo se incluya múltiples veces en la compilación
#include <memory>
#include "Position.hpp"

// "Forward declaration" (Declaración anticipada) de Piece. 
// Le dice al compilador "Piece es una clase", sin necesidad de incluir todo Piece.h aquí.

class Piece;

// Esto sirve para saber que tipo de movimiento se esta haciendo
enum class MoveType {
    NORMAL,
    EN_PASSANT,
    CASTLING,
    PROMOTION
};

// Esto se usa para saber a que pieza quiere coronar el peon
enum class PromotionType {
    NONE,
    QUEEN,
    ROOK,
    BISHOP,
    KNIGHT
};


class Move
{
    private:
        Position from;
        Position to;
        std::unique_ptr<Piece> capturedPiece;
        MoveType type;
        PromotionType promotionType;

        // Puntaje heurístico temporal usado para ordenar los movimientos en el Minimax
        int sortScore;

    public:
        //Constructor
        Move(Position f, Position t,
             MoveType type = MoveType::NORMAL,
             PromotionType pType = PromotionType::NONE);

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

        MoveType getType() const;
        PromotionType getPromotionType() const;

        // Esto se añade para en un futuro usarse en el algoritmo Minimax Poda Alpha-Beta
        int getSortScore() const;
        void setSortScore(int score);
};








