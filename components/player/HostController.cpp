//
// Created by Suraj Lyons on 04/03/2021.
//

#include "HostController.h"
#include "../util/io.h"

bool HostController::hasEitherPlayerLost() {
    return playerOneHost->hasPlayerLostAllShips() || playerTwo->hasPlayerLostAllShips();
}

void HostController::renderWinConditionInterface() {
    clearConsole();
    if (playerOneHost->hasPlayerLostAllShips()){
        std::cout << "The player \033[1;33m" << playerOneHost << "\033[0m has lost";
    } else {
        std::cout << "The player \033[1;33m" << playerTwo << "\033[0m has lost";
    }
}

