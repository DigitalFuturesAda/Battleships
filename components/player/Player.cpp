//
// Created by Suraj Lyons on 01/03/2021.
//

#include "Player.h"
#include <iostream>
#include <absl/strings/str_format.h>
#include <absl/strings/ascii.h>
#include "../../components/grid/GameGrid.h"

Player::Player() {
    this->playerShips = {
            Ship(CARRIER, true),
            Ship(BATTLESHIP, false),
            Ship(DESTROYER, true),
            Ship(SUBMARINE, true),
            Ship(PATROL, false),
    };

    this->battleshipGameGrid;
};

bool Player::attemptToDeployShip(Ship) {
    return false;
}

std::string Player::getStationaryShips() {
    int counter = 0;
    std::ostringstream stringStream;

//    stringStream << "\033[1;32mStationary ships:\033[0m ";
    stringStream << "Stationary ships: ";
    for (auto &&ship : playerShips){
        if (!ship.isDeployed()){
            stringStream << ship.getName();
            if (counter < playerShips.size() - 1){
                stringStream << ", ";
            }
            counter ++;
        }
    }
    if (counter == 0){
        stringStream << "None";
    }
//    stringStream << std::endl;

    return stringStream.str();
}

std::string Player::getDeployedShips() {
    int counter = 0;
    std::ostringstream stringStream;

//    stringStream << "\033[1;31mDeployed ships:\033[0m ";
    stringStream << "Deployed ships: ";
    for (auto &&ship : playerShips){
        if (ship.isDeployed()){
            stringStream << ship.getName();
            if (counter < playerShips.size() - 1){
                stringStream << ", ";
            }
            counter ++;
        }
    }
    if (counter == 0){
        stringStream << "None";
    }
//    stringStream << std::endl;

    return stringStream.str();
}

std::string Player::getShipData() {
    return getStationaryShips() +  getDeployedShips();
}