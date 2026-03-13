
#include <iostream>
#include "include/Game.hpp"
#include "include/HumanPlayer.hpp"

// Includes necesarios para printBoard
#include "include/Pawn.hpp"
#include "include/Rook.hpp"
#include "include/Knight.hpp"
#include "include/Bishop.hpp"
#include "include/Queen.hpp"
#include "include/King.hpp"

// La funcion de impresion se queda en el main (nuestra "interfaz de consola")
void printBoard(const Board& board) {
    std::cout << "\n  a b c d e f g h\n";
    for (int r = 0; r < 8; r++) {
        std::cout << 8 - r << " ";
        for (int c = 0; c < 8; c++) {
            Piece* p = board.getPieceAt(Position(r, c));
            if (p == nullptr) {
                std::cout << ". ";
            } else {
                char base = '?';
                if (dynamic_cast<Pawn*>(p)) base = 'P';
                else if (dynamic_cast<Rook*>(p)) base = 'R';
                else if (dynamic_cast<Knight*>(p)) base = 'N';
                else if (dynamic_cast<Bishop*>(p)) base = 'B';
                else if (dynamic_cast<Queen*>(p)) base = 'Q';
                else if (dynamic_cast<King*>(p)) base = 'K';

                if (p->getColor() == Color::BLACK) base += 32;
                std::cout << base << " ";
            }
        }
        std::cout << 8 - r << "\n";
    }
    std::cout << "  a b c d e f g h\n\n";
}

int main() {
    std::cout << "=== BIENVENIDO A CHESS AI ===\n";

    HumanPlayer whitePlayer(Color::WHITE);
    HumanPlayer blackPlayer(Color::BLACK);

    Game chessGame(&whitePlayer, &blackPlayer);

    // Bucle de la Interfaz de Usuario
    bool isRunning = true;
    while (isRunning) {
        // 1. Dibujamos el tablero pidiéndoselo al juego
        printBoard(chessGame.getBoard());

        // 2. Le decimos al juego que procese un turno
        // playTurn devolvera false si alguien ganó o hubo empate
        isRunning = chessGame.playTurn();
    }

    std::cout << "Gracias por jugar.\n";
    return 0;
}
