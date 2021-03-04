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
#include "../util/rand.h"

Player::Player(std::string _playerName) {
    this->playerShips = {
            Ship(CARRIER, false),
//            Ship(BATTLESHIP, false),
//            Ship(DESTROYER, false),
//            Ship(SUBMARINE, false),
//            Ship(PATROL, false),
    };
    this->playerName = std::move(_playerName);
};

attemptPlacementResponse Player::deployShip(int position, const std::string& letterIndex, int y, Orientation orientation) {
    if (position > playerShips.size() || position < 0){
        return attemptPlacementResponse(false, "Invalid position");
    }

    Ship ship = playerShips.at(position);

    if (ship.isDeployed()){
        return attemptPlacementResponse(false, "Ships already deployed");
    }

    attemptPlacementResponse response = getGameGrid()->attemptPlacement(letterIndex, y, ship.getShipType(), orientation);
    if (response.success){
        playerShips.at(position) = Ship(playerShips.at(position).getShipType(), true).setOrientation(orientation);
    }

    return response;
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

    if (y == 0) y++; // We don't like 0's round here.

    attemptPlacementResponse playerHitBoardResponse = getHitGrid()->checkIfNodeExists(letter, y);
    if (playerHitBoardResponse.existingNode.node == VALID_HIT || playerHitBoardResponse.existingNode.node == INVALID_HIT){
        return attemptHitResponse(false, "Cannot fire twice at the same node");
    }

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
    int c = 0;
    for (auto &&ship : playerShips){
        if (!ship.isDeployed()){
            deployShipInterface(c);
        }
        c++;
    }
}

bool Player::deployShipInterface(int shipVertexPosition){
    attemptPlacementResponse response;
    Ship ship = playerShips.at(shipVertexPosition);

    while (!response.success){
        regexMatch coordinateInput = getRegexInputWithPromptAsRegex(
                "Enter where you want to move your " + ship.getName() + " (eg. A1 or H8): ",
                std::regex("([A-a-Z-z](?:[A-a-B-b])?)([1-9](?:[1-10])?)"));

        std::string orientation = getRegexInputWithPromptAsString(
                "Enter ship orientation (vertical/horizontal): ",
                std::regex("(\\b[Vv][Ee][Rr][Tt][Ii][Cc][Aa][Ll]|[Hh][Oo][Rr][Ii][Zz][Oo][Nn][Tt][Aa][Ll]|[Vv]\\b|[Hh]\\b)"));

        int yCoordinate = stoi(coordinateInput.matches[1]);

        response = deployShip(shipVertexPosition, convertToUpperCase(coordinateInput.matches[0]),
                yCoordinate, convertToUpperCase(orientation).at(0) == 'V' ? VERTICAL : HORIZONTAL);

        if (!response.success){
            displayError("Failed to deploy ship - " + response.message + "\n", 2);
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

    if (alsoRenderComputerBoard){
        playerBattleshipGameTable.add_row({opposingPlayer->playerName + "'s Game board", opposingPlayer->playerName + "'s Hit board"});
        playerBattleshipGameTable.add_row({opposingPlayer->getGameGrid()->renderGrid(), opposingPlayer->getHitGrid()->renderGrid()});
    }

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

    std::cout << "\033[1;31m⋅ " << playerName << "'s ship status board\033[0m" << std::endl;
    std::cout << playerStatisticsBoard << std::endl;

    if (alsoRenderComputerBoard){
        // Add some spacing between the boards
        std::cout << std::endl;
        opposingPlayer->renderStatisticsBoard();
    }
}

void Player::renderPlayerUserInterface() {
    clearConsole();

    renderPlayerGrid();
    renderStatisticsBoard();
}

void Player::setPlayingAgainstComputer() {
    alsoRenderComputerBoard = true;
}

bool Player::deployWarshipAutomatically(int shipVertexPosition, int attempts) {
    Ship ship = playerShips.at(shipVertexPosition);

    if (ship.isDeployed()){
        return true;
    }

    if (attempts == MAX_SHIP_DEPLOYMENT_ATTEMPTS){
        return false;
    }

    Orientation randomOrientation = randomBool() ? VERTICAL : HORIZONTAL;

    int length = ship.getMaxLives();
    int gridHeight = GameGrid::HEIGHT;
    int gridWidth = GameGrid::WIDTH;

    attemptPlacementResponse response;

    if (randomOrientation == VERTICAL){
        int validPositionLow = 0;
        int validPositionHigh = gridHeight - length;

        int randomXCoordinate = randomBetween(validPositionLow - 1, validPositionHigh + 1);
        int randomYCoordinate = randomBetween(validPositionLow, gridWidth);

        response = deployShip(shipVertexPosition, convertIncrementingIntegerToAlpha(randomYCoordinate),
                              randomXCoordinate, randomOrientation);
    } else {
        int validPositionLow = 0;
        int validPositionHigh = gridWidth - length;

        int randomYCoordinate = randomBetween(validPositionLow, validPositionHigh);
        int randomXCoordinate = randomBetween(validPositionLow, gridHeight);

        response = deployShip(shipVertexPosition, convertIncrementingIntegerToAlpha(randomYCoordinate),
                              randomXCoordinate, randomOrientation);
    }

    if (response.success == false){
        return deployWarshipAutomatically(shipVertexPosition, attempts + 1);
    }

    return true;
}

std::vector<Ship> *Player::getPlayerShips() {
    return &playerShips;
}

void Player::deployWarshipsAutomatically() {
    int c = 0;
    for (auto &&ship : playerShips){
        bool shipPlacementAttempt = deployWarshipAutomatically(c);
        if (!shipPlacementAttempt){
            displayError("Failed to place ships within the board, please reduce the amount of ships or increase the board size", 0);
            exit (EXIT_FAILURE);
        }
        c++;
    }
}

void Player::renderWarheadStrikeInterface() {
    regexMatch coordinateInput = getRegexInputWithPromptAsRegex(
            "Enter a coordinate to deploy your warhead strike (eg. A1 or H8): ",
            std::regex("([A-a-Z-z](?:[A-a-B-b])?)([1-9](?:[1-10])?)"));

    int yCoordinate = stoi(coordinateInput.matches[1]);
    attemptHitResponse response = executeWarheadStrike(convertToUpperCase(coordinateInput.matches[0]), yCoordinate);

    if (response.validAttempt){
        // Update the board with the hit
        renderPlayerUserInterface();
        if (response.didHitTarget){
            displayInformation("Successful warhead strike - hit a " + Ship(response.hitNode.node).getName() + "\n", 0);
        } else {
            displayInformation("Unsuccessful warhead strike - did not hit anything\n", 0);
        }
    } else {
        displayError(response.message + ": ", 1);
        return renderWarheadStrikeInterface();
    }
    awaitBlankInput();
}

attemptHitResponse Player::deployWarheadStrikeAutomatically(int attempts) {
    // TODO(slyo): Verify all ships not destroyed. Maybe for the purposes of this game we can assume the computer has
    //  rudimentary knowledge of the approx bounds of players ships location as to decrease trial and error.

    int gridHeight = GameGrid::HEIGHT;
    int gridWidth = GameGrid::WIDTH;
    attemptHitResponse hitResponse;

    while (!hitResponse.validAttempt){
        int randomXCoordinate = randomBetween19937(0, gridWidth);
        int randomYCoordinate = randomBetween19937(0, gridHeight);

        std::string letter = convertToUpperCase(convertIncrementingIntegerToAlpha(randomXCoordinate));
        hitResponse = executeWarheadStrike(letter, randomYCoordinate);

        if (attempts == MAX_WARHEAD_STRIKES_ATTEMPTS){
            displayError("Max strikes attempted reached, please increase the board size", 0);
            exit (EXIT_FAILURE);
        }

        attempts ++;
    }

    return hitResponse;
}

bool Player::hasPlayerLostAllShips() {
    for (auto&& ship : playerShips){
        if (!ship.isSunk()){
            return false;
        }
    }
    return true;
}