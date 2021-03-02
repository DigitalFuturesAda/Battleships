//
// Created by Suraj Lyons on 01/03/2021.
//

#include "Player.h"
#include <iostream>
#include <utility>
#include <absl/strings/str_format.h>
#include <absl/strings/ascii.h>
#include "../../components/grid/GameGrid.h"
#include "../util/strings.h"

Player::Player() {
    this->playerShips = {
            Ship(CARRIER, false),
            Ship(BATTLESHIP, false),
            Ship(DESTROYER, false),
            Ship(SUBMARINE, false),
            Ship(PATROL, false),
    };
};

attemptPlacementResponse Player::deployShip(GridNodes shipType, const std::string& letterIndex, int y, Orientation orientation) {
    int counter = 0;
    for (auto &ship : playerShips){
        if (ship.getShipType() == shipType && ship.isDeployed() == false){
            attemptPlacementResponse response = getGameGrid()->attemptPlacement(letterIndex, y, shipType, orientation);

            if (response.success == true){
                playerShips.at(counter) = Ship(shipType, true).setOrientation(orientation);
            }

            return response;
        }

        counter ++;
    }

    return attemptPlacementResponse(false, "All ships already deployed.");
}

std::string Player::getStationaryShips() {
    int counter = 0;
    std::ostringstream stringStream;

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

void Player::setOpposingPlayer(Player *player) {
    this->opposingPlayer = player;
}

attemptHitResponse Player::executeWarheadStrike(std::string letter, int y) {
    attemptHitResponse response = opposingPlayer->battleshipGameGrid.receiveWarheadStrike(letter, y);

    if (response.validAttempt){
        if (response.didHitTarget){
            battleshipHitGrid.markSuccessfulWarheadStrike(response.hitNode.x, response.hitNode.y);
            attemptPlacementResponse responseTest = opposingPlayer->getGameGrid()->attemptPlacement(letter, y, DESTROYED, VERTICAL);
            int counter = 0;
            for (auto &ship : opposingPlayer->playerShips){
                if (ship.getShipType() == responseTest.existingNode.node){
                    Ship replacementShip = Ship(responseTest.existingNode.node, true);
                    replacementShip.setLives(ship.getLives() - 1);
                    opposingPlayer->playerShips.at(counter) = replacementShip;
                }

                counter++;
            }
        } else {
            battleshipHitGrid.markFailedWarheadStrike(response.hitNode.x, response.hitNode.y);
            return response;
        }
    } else {
        std::cout << "Invalid attempt!" << std::endl;
        return response;
    }

    return response;
}

GameGrid *Player::getGameGrid() {
    return &battleshipGameGrid;
}

HitGrid *Player::getHitGrid() {
    return &battleshipHitGrid;
}

std::vector<std::string> Player::getShipInformation(){
    std::ostringstream stringStream[3];

    for (auto &&ship : playerShips){
        stringStream[0] << ship.getName() << std::endl;
        stringStream[1] << ship.getShipStatusFormatted() << std::endl;
        stringStream[2] << ship.getLives() << " / " << ship.getMaxLives() << std::endl;
    }

    return {stringStream[0].str(), stringStream[1].str(), stringStream[2].str()};
}
