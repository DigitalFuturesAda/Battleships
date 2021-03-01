//
// Created by Suraj Lyons on 01/03/2021.
//

#include "Player.h"

Player::Player() {
    this->playerShips = {
            Ship(CARRIER, false),
            Ship(BATTLESHIP, false),
            Ship(DESTROYER, false),
            Ship(SUBMARINE, false),
            Ship(PATROL, false),
    };
};

void Player::outputStationaryShips() {
    int counter = 0;
    std::cout << "\033[1;32mStationary ships:\033[0m ";
    for (auto &&ship : playerShips){
        if (!ship.isDeployed()){
            std::cout << ship.getName();
            if (counter < playerShips.size() - 1){
                std::cout << ", ";
            }
            counter ++;
        }
    }
    if (counter == 0){
        std::cout << "None";
    }
    std::cout << std::endl;
}

bool Player::attemptToDeployShip(Ship) {
    return false;
}

void Player::outputShipData() {
    std::cout << std::endl;

    outputStationaryShips();
    outputDeployedShips();
}

void Player::outputDeployedShips() {
    int counter = 0;
    std::cout << "\033[1;31mDeployed ships:\033[0m ";
    for (auto &&ship : playerShips){
        if (ship.isDeployed()){
            std::cout << ship.getName();
            if (counter < playerShips.size() - 1){
                std::cout << ", ";
            }
            counter ++;
        }
    }
    if (counter == 0){
        std::cout << "None";
    }
    std::cout << std::endl;
}
