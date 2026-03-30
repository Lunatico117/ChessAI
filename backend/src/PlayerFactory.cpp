#include "../include/PlayerFactory.hpp"
#include "../include/HumanPlayer.hpp"
#include "../include/AIPlayer.hpp"

Player* PlayerFactory::createPlayer(const std::string& type, Color color, int difficulty) {
    if (type == "Human") {
        return new HumanPlayer(color);
    }
    else if (type == "AI") {
        return new AIPlayer(color, difficulty);
    }

    // Retorno por seguridad en caso de error
    return nullptr;
}
