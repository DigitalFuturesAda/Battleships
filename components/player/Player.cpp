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
#include "../util/io.h"

Player::Player(std::string _playerName) {
    this->playerShips = {
            Ship(CARRIER, false),
            Ship(BATTLESHIP, false),
            Ship(DESTROYER, false),
            Ship(SUBMARINE, false),
            Ship(PATROL, false),
    };
    this->playerName = std::move(_playerName);
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
    // TODO(slyo): Handle mines differently.
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

void Player::showShipDeploymentInterface() {
    for (auto &&ship : playerShips){
        deployShipInterface(ship);
    }
}

bool Player::deployShipInterface(Ship ship){
    attemptPlacementResponse response;

    while (!response.success){
        regexMatch coordinateInput = getRegexInputWithPromptAsRegex(
                "Enter where you want to move your " + ship.getName() + " (eg. A1 or H8): ",
                std::regex("([A-a-Z-z](?:[A-a-B-b])?)([1-9](?:[1-10])?)"));

        std::string orientation = getRegexInputWithPromptAsString(
                "Enter ship orientation (vertical/horizontal): ",
                std::regex("(\\b[Vv][Ee][Rr][Tt][Ii][Cc][Aa][Ll]|[Hh][Oo][Rr][Ii][Zz][Oo][Nn][Tt][Aa][Ll]|[Vv]\\b|[Hh]\\b)"));

        int yCoordinate = stoi(coordinateInput.matches[1]);

        response = deployShip(
                ship.getShipType(),
                convertToUpperCase(coordinateInput.matches[0]),
                yCoordinate,
                convertToUpperCase(orientation).at(0) == 'V' ? VERTICAL : HORIZONTAL);

        if (!response.success){
            std::cout << "Failed to deploy ship. Error: " << response.message << std::endl;
        } else {
            renderPlayerUserInterface();
        }
    }

    return response.success;
}

void Player::renderPlayerGrid() {
    tabulate::Table playerBattleshipGameTable;
    playerBattleshipGameTable.format()
            .border_color(tabulate::Color::white)
            .corner("⋅")
            .corner_color(tabulate::Color::red);


    playerBattleshipGameTable.add_row({playerName + "'s Game board", playerName + "'s Hit board"});

    playerBattleshipGameTable.column(0).format().width(GameGrid::getObservableGridWidth());
    playerBattleshipGameTable.column(1).format().width(GameGrid::getObservableGridWidth());

    playerBattleshipGameTable.add_row({getGameGrid()->renderGrid(), getHitGrid()->renderGrid()});

    std::cout << playerBattleshipGameTable << std::endl;
}

void Player::renderStatisticsBoard() {
    tabulate::Table playerStatisticsBoard;
    playerStatisticsBoard.format()
            .border_color(tabulate::Color::white)
            .corner("⋅")
            .corner_color(tabulate::Color::red);

    playerStatisticsBoard.add_row({"Boat", "Status", "Durability"});
    playerStatisticsBoard.add_row({
        getShipInformation().at(0),
        getShipInformation().at(1),
        getShipInformation().at(2)});

    std::cout << playerStatisticsBoard << std::endl;
}

void Player::renderPlayerUserInterface() {
    clearConsole();
    renderPlayerGrid();
    renderStatisticsBoard();
}