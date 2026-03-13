/*
#include <iostream>
#include <vector>
#include "include/GameState.hpp"
#include "include/MoveGenerator.hpp"
#include "include/RuleValidator.hpp"
#include "include/HumanPlayer.hpp"

// Incluimos todas las piezas para el dynamic_cast
#include "include/Pawn.hpp"
#include "include/Rook.hpp"
#include "include/Knight.hpp"
#include "include/Bishop.hpp"
#include "include/Queen.hpp"
#include "include/King.hpp"

// (MANTÉN AQUÍ TU FUNCIÓN printBoard ACTUAL)
// void printBoard(const Board& board) { ... }

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


int main() {
    std::cout << "=== BIENVENIDO A CHESS AI ===\n";

    GameState state;
    MoveGenerator generator;
    RuleValidator validator;

    // 1. Instanciamos a los dos jugadores
    HumanPlayer whitePlayer(Color::WHITE);
    HumanPlayer blackPlayer(Color::BLACK);

    bool gameOver = false;

    // 2. El Bucle de Juego
    while (!gameOver) {
        printBoard(state.getBoard());

        Color currentTurn = state.getCurrentTurn();

        // Generamos los movimientos legales para el turno actual
        std::vector<Move> legalMoves = generator.generateLegalMoves(state);

        //Imprimir movimientos legales ---
        /*std::cout << "\nMovimientos legales disponibles:\n";
        for (const Move& m : legalMoves) {
            // Obtenemos las coordenadas de la matriz
            int fromR = m.getFrom().getRow();
            int fromC = m.getFrom().getCol();
            int toR = m.getTo().getRow();
            int toC = m.getTo().getCol();

            // Convertimos la matriz (0-7) de vuelta a texto de ajedrez (a-h, 8-1)
            char fromColChar = 'a' + fromC;
            char fromRowChar = '8' - fromR;
            char toColChar = 'a' + toC;
            char toRowChar = '8' - toR;

            std::cout << "- " << fromColChar << fromRowChar << " " << toColChar << toRowChar << "\n";
        }
        std::cout << "--------------------------------\n";
        // -------------------------------------------
        */
        // 3. Condición de Fin de Juego (Jaque Mate o Ahogado)
        if (legalMoves.empty()) {
            if (validator.isKingInCheck(state, currentTurn)) {
                std::cout << "\n¡JAQUE MATE! Gana el jugador "
                          << (currentTurn == Color::WHITE ? "NEGRO" : "BLANCO") << ".\n";
            } else {
                std::cout << "\n¡TABLAS por Rey Ahogado!\n";
            }
            gameOver = true;
            break; // Salimos del bucle
        }

        // 4. Determinar de quién es el turno usando Polimorfismo
        Player* currentPlayer = (currentTurn == Color::WHITE) ?
                                    static_cast<Player*>(&whitePlayer) :
                                    static_cast<Player*>(&blackPlayer);

        // 5. El jugador piensa y elige un movimiento
        Move chosenMove = currentPlayer->getMove(state, legalMoves);

        // 6. Ejecutamos el movimiento en el estado global
        state.updateState(chosenMove);
    }

    std::cout << "Gracias por jugar.\n";
    return 0;
}
*/
