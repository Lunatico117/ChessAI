// Basado en la Función de Evaluación Simplificada de Michniewski
#pragma once

// Añadimos los includes necesarios para que reconozca los tipos de datos
#include "Piece.hpp"    // Para PieceType y Color
#include "Position.hpp" // Para Position


// Usamos un namespace para mantener todo organizado y no ensuciar el espacio global
namespace PST {

// Use constexpr int en lugar de solo const int porque obliga al compilador a
// evaluar estas matrices en tiempo de compilacion

// -----------------------------------------------------------------------
// PEONES: Premia avanzar hacia la coronación (Fila 1 y 2) y dominar el centro.
// Penaliza dejar los peones centrales atrás (Fila 6).
// -----------------------------------------------------------------------
constexpr int pawnTable[8][8] = {
    {  0,  0,  0,  0,  0,  0,  0,  0}, // Fila 0 (Coronación - El peón deja de ser peón)
    { 50, 50, 50, 50, 50, 50, 50, 50}, // Fila 1 (A un paso de coronar, ¡muy valioso!)
    { 10, 10, 20, 30, 30, 20, 10, 10}, // Fila 2
    {  5,  5, 10, 25, 25, 10,  5,  5}, // Fila 3
    {  0,  0,  0, 20, 20,  0,  0,  0}, // Fila 4 (Centro)
    {  5, -5,-10,  0,  0,-10, -5,  5}, // Fila 5
    {  5, 10, 10,-20,-20, 10, 10,  5}, // Fila 6 (Inicio. -20 motiva a mover los peones d y e)
    {  0,  0,  0,  0,  0,  0,  0,  0}  // Fila 7
};

// -----------------------------------------------------------------------
// CABALLOS: Odian las esquinas (-50) y aman el centro (+30).
// Un caballo en el centro controla todo el tablero.
// -----------------------------------------------------------------------
constexpr int knightTable[8][8] = {
    {-50,-40,-30,-30,-30,-30,-40,-50},
    {-40,-20,  0,  0,  0,  0,-20,-40},
    {-30,  0, 10, 15, 15, 10,  0,-30},
    {-30,  5, 15, 20, 20, 15,  5,-30},
    {-30,  0, 15, 20, 20, 15,  0,-30},
    {-30,  5, 10, 15, 15, 10,  5,-30},
    {-40,-20,  0,  5,  5,  0,-20,-40},
    {-50,-40,-30,-30,-30,-30,-40,-50}
};

// -----------------------------------------------------------------------
// ALFILES: Evitan las esquinas, buscan diagonales largas y se desarrollan
// preferiblemente lejos de su fila inicial.
// -----------------------------------------------------------------------
constexpr int bishopTable[8][8] = {
    {-20,-10,-10,-10,-10,-10,-10,-20},
    {-10,  0,  0,  0,  0,  0,  0,-10},
    {-10,  0,  5, 10, 10,  5,  0,-10},
    {-10,  5,  5, 10, 10,  5,  5,-10},
    {-10,  0, 10, 10, 10, 10,  0,-10},
    {-10, 10, 10, 10, 10, 10, 10,-10},
    {-10,  5,  0,  0,  0,  0,  5,-10},
    {-20,-10,-10,-10,-10,-10,-10,-20}
};

// -----------------------------------------------------------------------
// TORRES: Les encanta la séptima fila enemiga (Fila 1 para las Blancas)
// y dominar las columnas centrales abiertas.
// -----------------------------------------------------------------------
constexpr int rookTable[8][8] = {
    {  0,  0,  0,  0,  0,  0,  0,  0},
    {  5, 10, 10, 10, 10, 10, 10,  5}, // Séptima fila enemiga (¡Poder total!)
    { -5,  0,  0,  0,  0,  0,  0, -5},
    { -5,  0,  0,  0,  0,  0,  0, -5},
    { -5,  0,  0,  0,  0,  0,  0, -5},
    { -5,  0,  0,  0,  0,  0,  0, -5},
    { -5,  0,  0,  0,  0,  0,  0, -5},
    {  0,  0,  0,  5,  5,  0,  0,  0}  // Fila inicial, bonus por centralizarse (d1, e1)
};

// -----------------------------------------------------------------------
// REINAS: Generalmente se penaliza sacarlas demasiado temprano o ponerlas
// en las esquinas. Preferible que estén cerca del centro pero protegidas.
// -----------------------------------------------------------------------
constexpr int queenTable[8][8] = {
    {-20,-10,-10, -5, -5,-10,-10,-20},
    {-10,  0,  0,  0,  0,  0,  0,-10},
    {-10,  0,  5,  5,  5,  5,  0,-10},
    { -5,  0,  5,  5,  5,  5,  0, -5},
    {  0,  0,  5,  5,  5,  5,  0, -5},
    {-10,  5,  5,  5,  5,  5,  0,-10},
    {-10,  0,  5,  0,  0,  0,  0,-10},
    {-20,-10,-10, -5, -5,-10,-10,-20}
};

// -----------------------------------------------------------------------
// REY (Medio Juego): El rey DEBE esconderse detrás de sus peones.
// Se premia enormemente el enroque (g1 y c1 en Fila 7) y se castiga severamente
// avanzar al centro en las etapas tempranas de la partida.
// -----------------------------------------------------------------------
constexpr int kingTable[8][8] = {
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-20,-30,-30,-40,-40,-30,-30,-20},
    {-10,-20,-20,-20,-20,-20,-20,-10},
    { 20, 20,  0,  0,  0,  0, 20, 20}, // Escudo de peones delante del enroque
    { 20, 30, 10,  0,  0, 10, 30, 20}  // Enroque (g1 es +30, c1 es +10)
};

inline int getPositionalBonus(PieceType type, Color color, const Position& pos) {
    int r = pos.getRow();
    int c = pos.getCol();

    // Corrección de perspectiva:
    // Las tablas están hechas pensando en las Blancas (Fila 7 a 0).
    // Si la pieza es Negra, invertimos la fila (0 a 7).
    if (color == Color::BLACK) {
        r = 7 - r;
    }

    // Devolvemos el valor de la matriz correspondiente en O(1)
    switch (type) {
    case PieceType::PAWN:   return pawnTable[r][c];
    case PieceType::KNIGHT: return knightTable[r][c];
    case PieceType::BISHOP: return bishopTable[r][c];
    case PieceType::ROOK:   return rookTable[r][c];
    case PieceType::QUEEN:  return queenTable[r][c];
    case PieceType::KING:   return kingTable[r][c];
    default: return 0;
    }
}

}
// fin del namespace PST
