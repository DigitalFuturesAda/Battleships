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
        std::cout << "Player: \033[1;31m" << playerOneHost->playerName << "\033[0m has lost the game!" << std::endl;
        std::cout << "Congratulations: \033[1;31m" << playerTwo->playerName << "\033[0m you have won!" << std::endl;
    } else {
        std::cout << "Player: \033[1;31m" << playerTwo->playerName << "\033[0m has lost!" << std::endl;
        std::cout << "Congratulations: \033[1;31m" << playerOneHost->playerName << "\033[0m you have won!" << std::endl;
    }
}

