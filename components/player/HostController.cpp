//
// Created by Suraj Lyons on 04/03/2021.
//

#include "HostController.h"
#include "../util/io.h"
#include <iostream>
#include <fstream>

bool HostController::hasEitherPlayerLost() {
    return playerOneHost->hasPlayerLostAllShips() || playerTwo->hasPlayerLostAllShips();
}

void HostController::renderWinConditionInterface() {
    clearConsole();

    std::string winningPlayer = std::to_string(playerOneHost->numberOfAttempts);

    if (playerOneHost->hasPlayerLostAllShips()){
        std::cout << "Player: \033[1;31m" << playerOneHost->playerName << "\033[0m has lost the game!" << std::endl;
        std::cout << "Congratulations: \033[1;31m" << playerTwo->playerName << "\033[0m you have won!" << std::endl;
        std::cout << "In " << playerTwo->numberOfAttempts << " attempts!" << std::endl;
    } else {
        std::cout << "Player: \033[1;31m" << playerTwo->playerName << "\033[0m has lost!" << std::endl;
        std::cout << "Congratulations: \033[1;31m" << playerOneHost->playerName << "\033[0m you have won!" << std::endl;
        std::cout << "In " << playerOneHost->numberOfAttempts << " attempts!" << std::endl;
    }

    // Required for collecting statistics
    std::ofstream winningPlayerStatisticsFile;
    winningPlayerStatisticsFile.open ("winningPlayerStatisticsFile.csv", std::ios_base::app);
    winningPlayerStatisticsFile << winningPlayer << "\n";
    winningPlayerStatisticsFile.close();
    std::cout << "Wrote " << winningPlayer << " to winningPlayerStatisticsFile.csv" << std::endl;
}
