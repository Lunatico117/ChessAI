// Es el encargado de crear los jugadores
// Sigue el patron Factory
#pragma once
#include "Player.hpp"
#include <string>

class PlayerFactory {
public:
    // Método estático: no necesitamos crear una instancia de la fábrica para usarla
    static Player* createPlayer(const std::string& type, Color color, int difficulty = 1);
};
