# include "../include/HumanPlayer.hpp"
#include <iostream>
#include <limits> // Necesario para limpiar el buffer

HumanPlayer::HumanPlayer(Color c): Player(c){}

Position HumanPlayer::parsePosition (const std::string& input) const {

    // Verificacion del mensaje enviado por el usuario
    if (input.length() < 2){
        return Position(-1, -1);
    }

    // input[0] es la letra (ej. 'e'). 'e' - 'a' = 4
    int col = input[0] - 'a';

    // 1. Convertimos el caracter (ej. '2') a un número real (2) restando '0'
    int rank = input[1] - '0';

    // 2. Invertimos para la matriz (Fila 8 de ajedrez es índice 0 en matriz)
    int row = 8 - rank;

    return Position(row, col);
}


Move HumanPlayer::getMove(const GameState& state, const std::vector<Move>& legalMoves) {
    std::string fromStr, toStr;

    while (true) {
        std::cout << "\nEs el turno del jugador " << (color == Color::WHITE ? "BLANCO" : "NEGRO") << ".\n";
        std::cout << "Ingresa la casilla de origen (ej. e2) y destino (ej. e4): ";

        // Intentamos leer. Si std::cin falla por alguna razón...
        if (!(std::cin >> fromStr >> toStr)) {
            std::cout << "Error en la consola. Limpiando buffer...\n";
            std::cin.clear(); // Quitamos el estado de error de cin
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Borramos la basura
            continue; // Volvemos al inicio del bucle
        }

        Position from = parsePosition(fromStr);
        Position to = parsePosition(toStr);

        // Verificamos si la jugada está en la lista de permitidas
        for (const Move& legalMove : legalMoves) {
            if (legalMove.getFrom() == from && legalMove.getTo() == to) {
                return Move(legalMove.getFrom(), legalMove.getTo());
            }
        }

        std::cout << "Movimiento invalido o ilegal (" << fromStr << " -> " << toStr << "). Por favor intenta de nuevo.\n";
    }
}







