//
// Created by Suraj Lyons on 01/03/2021.
//

#include "Player.h"
#include <iostream>
#include <utility>
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

//    this->battleshipGameGrid;
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

    return stringStream.str();
}

std::string Player::getDeployedShips() {
    int counter = 0;
    std::ostringstream stringStream;

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

    return stringStream.str();
}

std::string Player::getShipData() {
    return getStationaryShips() +  getDeployedShips();
}

void Player::setOpposingPlayer(Player *player) {
    this->opposingPlayer = player;
}

attemptHitResponse Player::fireWarheadStrikeAtOpposingPlayer(std::string letter, int number) {
    attemptHitResponse response = opposingPlayer->battleshipGameGrid.receiveWarheadStrike(std::move(letter), number);

    if (response.validAttempt){
        if (response.didHitTarget){
            battleshipHitGrid.markSuccessfulWarheadStrike(response.hitNode.x, response.hitNode.y);
        } else {
            battleshipHitGrid.markFailedWarheadStrike(response.hitNode.x, response.hitNode.y);
        }
    } else {
        std::cout << "Invalid attempt!" << std::endl;
    }

    return attemptHitResponse(true, "true");
}

GameGrid *Player::getGameGrid() {
    return &battleshipGameGrid;
}

HitGrid *Player::getHitGrid() {
    return &battleshipHitGrid;
}
