/*
#include <iostream>
#include <vector>

// Para probarlo en el main retirar el ../
#include "include/GameState.hpp"
#include "include/MoveGenerator.hpp"
#include "include/Piece.hpp"

// Incluimos todas las piezas para el dynamic_cast
#include "include/Pawn.hpp"
#include "include/Rook.hpp"
#include "include/Knight.hpp"
#include "include/Bishop.hpp"
#include "include/Queen.hpp"
#include "include/King.hpp"

// PRUEBA DE FUNCIONAMIENTO DE LAS CLASES


// Función auxiliar para imprimir el tablero en la consola
void printBoard(const Board& board) {
    std::cout << "\n  a b c d e f g h\n";
    for (int r = 0; r < 8; r++) {
        std::cout << 8 - r << " "; // Numero de fila
        for (int c = 0; c < 8; c++) {
            Piece* p = board.getPieceAt(Position(r, c));
            if (p == nullptr) {
                std::cout << ". ";
            } else {
                char base = '?';

                // Identificamos explícitamente cada pieza
                if (dynamic_cast<Pawn*>(p)) base = 'P';
                else if (dynamic_cast<Rook*>(p)) base = 'R';
                else if (dynamic_cast<Knight*>(p)) base = 'N';
                else if (dynamic_cast<Bishop*>(p)) base = 'B';
                else if (dynamic_cast<Queen*>(p)) base = 'Q';
                else if (dynamic_cast<King*>(p)) base = 'K';

                // Mayúscula = Blanco, Minúscula = Negro
                if (p->getColor() == Color::BLACK) base += 32;
                std::cout << base << " ";
            }
        }
        std::cout << 8 - r << "\n";
    }
    std::cout << "  a b c d e f g h\n\n";
}

// Función auxiliar para traducir coordenadas a texto (ej. 7,4 -> e1)
std::string moveToText(const Move& m) {
    char fromCol = 'a' + m.getFrom().getCol();
    int fromRow = 8 - m.getFrom().getRow();
    char toCol = 'a' + m.getTo().getCol();
    int toRow = 8 - m.getTo().getRow();
    return std::string(1, fromCol) + std::to_string(fromRow) + " -> " + std::string(1, toCol) + std::to_string(toRow);
}

int main() {
    std::cout << "=== INICIANDO PRUEBA: JAQUE MATE AL PASTOR ===\n";

    GameState state;
    MoveGenerator generator;

    std::cout << "\nTablero Inicial:";
    printBoard(state.getBoard());

    // 1. Blanco mueve Peón de Rey e2 -> e4
    Move m1(Position(6, 4), Position(4, 4));
    state.updateState(m1);
    std::cout << "=== TURNO 1: Blanco mueve e4 ===";
    printBoard(state.getBoard());

    // 1... Negro mueve Peón de Rey e7 -> e5
    Move m2(Position(1, 4), Position(3, 4));
    state.updateState(m2);
    std::cout << "=== TURNO 1: Negro mueve e5 ===";
    printBoard(state.getBoard());

    // 2. Blanco mueve Alfil f1 -> c4
    Move m3(Position(7, 5), Position(4, 2));
    state.updateState(m3);
    std::cout << "=== TURNO 2: Blanco mueve Ac4 ===";
    printBoard(state.getBoard());

    // 2... Negro mueve Caballo b8 -> c6
    Move m4(Position(0, 1), Position(2, 2));
    state.updateState(m4);
    std::cout << "=== TURNO 2: Negro mueve Cc6 ===";
    printBoard(state.getBoard());

    // 3. Blanco mueve Reina d1 -> h5
    Move m5(Position(7, 3), Position(3, 7));
    state.updateState(m5);
    std::cout << "=== TURNO 3: Blanco mueve Dh5 ===";
    printBoard(state.getBoard());

    // 3... Negro mueve Caballo g8 -> f6 (¡Grave error!)
    Move m6(Position(0, 6), Position(2, 5));
    state.updateState(m6);
    std::cout << "=== TURNO 3: Negro mueve Cf6 ===";
    printBoard(state.getBoard());

    // 4. Blanco mueve Reina h5 -> f7# (¡JAQUE MATE!)
    // Nota: La Reina captura el peón negro en f7
    Move m7(Position(3, 7), Position(1, 5));
    state.updateState(m7);
    std::cout << "=== TURNO 4: Blanco mueve Dxf7# (¡JAQUE MATE AL PASTOR!) ===";
    printBoard(state.getBoard());

    // --- VERIFICACIÓN DEL MATE ---
    std::vector<Move> legalMoves = generator.generateLegalMoves(state);

    std::cout << "=== ANALISIS FINAL ===\n";
    std::cout << "Turno actual: " << (state.getCurrentTurn() == Color::WHITE ? "BLANCO" : "NEGRO") << "\n";
    std::cout << "Movimientos LEGALES del Negro para intentar salvar al Rey: " << legalMoves.size() << "\n";

    if (legalMoves.empty()) {
        std::cout << "\n¡MATE CONFIRMADO! El Rey negro no tiene escapatoria y las demas piezas no pueden bloquear ni capturar a la Reina.\n";
    } else {
        std::cout << "\nALERTA: Tu generador encontro movimientos legales. Revisa isKingInCheck o generateLegalMoves.\n";
        std::cout << "Movimientos encontrados:\n";
        for (const Move& m : legalMoves) {
            std::cout << "- " << moveToText(m) << "\n";
        }
    }

    return 0;
}
*/
