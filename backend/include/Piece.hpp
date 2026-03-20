#pragma once // Evita que el archivo se incluya múltiples veces en la compilación

#include <vector>
#include "Move.hpp"
#include "Position.hpp"


// Declaracion anticipada (Forward declaration) para evitar dependencias circulares, ya que se quedarian leyendo los archivos de cada uno
// Lo cual arroja el error de Board no esta definido, ya que nunca terminara de leerlo
// Le decimos a Piece que existe un Board, sin tener que incluir todo Board.hpp, como una promesa
class Board;
class GameState;

// Usamos un enum class para mayor seguridad en C++ en lugar de ints sueltos
enum class Color {
    WHITE,
    BLACK
};

enum class PieceType {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};

class Piece{
    // Se protege y no se priva para que las clases hijas lo lean
    protected: 
        Color color;


    public:
        // Constructor
        Piece(Color c);

        // Destructor virtual. Se usa en clases abstractas obligatoriamente 
        // Esto hace que cuando se destruya una pieza se limpie por completo la memoria de sus subclases
        virtual ~Piece() = default;

        // Getter 
        Color getColor()const;

        // Metodo virtual puro ( =0) 
        // Esto lo convierte en clase abstracta 
        // Obliga a sus subclases crear sus propias reglas de movimiento
        virtual std::vector<Move> getPossibleMoves(const GameState& state, const Position& currentPos) const = 0;

        // Cada pieza tiene un valor distinto
        virtual int getValue() const = 0;

        // Se usa para la IA
        virtual PieceType getType() const = 0;

        // Se usa para el tablero
        // Cada pieza retorna su nombre para no usar dynamic_cast
        virtual std::string getTypeName() const = 0;
};
