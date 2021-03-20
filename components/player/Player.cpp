//
// Created by Suraj Lyons on 01/03/2021.
//

#include "Player.h"
#include <iostream>
#include <set>
#include <utility>
#include <algorithm>
#include <absl/strings/ascii.h>
#include "../../components/grid/GameGrid.h"
#include "../util/strings.h"
#include "../util/io.h"
#include "../util/rand.h"
#include "../config/ConfigSingleton.h"

Player::Player(std::string playerName, GameFlowController &gameFlowController) {
    initiatePlayerShipsVector();

    this->playerName = std::move(playerName);
    this->gameFlowController = &gameFlowController;

    for (int i = 0; i <= getGameGrid()->getGridWidth(); i++){
        for (int v = 0; v <= getGameGrid()->getGridHeight(); v++){
            potentialNodes.emplace_back(i, v);
        }
    }
}

void Player::initiatePlayerShipsVector(){
    for (auto&& shipData : ConfigSingleton::getInstance()->getValidator().getShipInventory())
        for (int i = 0; i < shipData.amountOfShips; i++)
            playerShips.emplace_back(Ship::shipNameToGridNode(shipData.shipType), false);
}

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
        playerShips.at(position) =
                Ship(playerShips.at(position)
                    .getShipType(), true)
                    .setOrientation(orientation)
                    .setShipCoordinatePositions(response.shipCoordinatePositions);
    }

    return response;
}

attemptPlacementResponse Player::deployMine(int x, int y) {
    // TODO(slyo): Add an assertion that all ships have been placed.
    // NOTE: You can not deploy a ship on a mine, but you can deploy a mine on a ship.

    return getGameGrid()->attemptPlacement(x, y, MINE, VERTICAL);
}

void Player::setOpposingPlayer(Player *player) {
    this->opposingPlayer = player;
}

HitNode hitNode(GridNodes hitNode){
    switch (hitNode){
        case EMPTY:
        case UNKNOWN:
        case DESTROYED:
        case VALID_HIT:
        case INVALID_HIT:
            return HIT_NODE_UNKNOWN;
        case CARRIER:
        case BATTLESHIP:
        case DESTROYER:
        case SUBMARINE:
        case PATROL:
            return HIT_NODE_SHIP;
        case MINE:
        case CARRIER_MINE:
        case BATTLESHIP_MINE:
        case DESTROYER_MINE:
        case SUBMARINE_MINE:
        case PATROL_MINE:
            return HIT_NODE_MINE;
    }
}

bool Player::isOutOfBounds(int x, int y){
    return (x < 1 || y < 1 || y > getGameGrid()->getGridHeight() || x > getGameGrid()->getGridWidth());
}

void Player::pushNodeAsAlphaIntoVector(int x, int y, std::vector<adjacentNodeEntry> *adjacentNodeEntries){
    if (isOutOfBounds(x,y)) return;
    adjacentNodeEntries->push_back(adjacentNodeEntry(convertIncrementingIntegerToAlpha(x), y));
}

std::vector<adjacentNodeEntry> Player::getAdjacentNodes(int x, int y){
    int currentNodeY = y + 1;
    int currentNodeX = x + 1;
    std::vector<adjacentNodeEntry> adjacentNodeEntries = {};

    pushNodeAsAlphaIntoVector(currentNodeX - 1, currentNodeY - 1, &adjacentNodeEntries);
    pushNodeAsAlphaIntoVector(currentNodeX - 1, currentNodeY, &adjacentNodeEntries);
    pushNodeAsAlphaIntoVector(currentNodeX - 1, currentNodeY + 1, &adjacentNodeEntries);

    pushNodeAsAlphaIntoVector(currentNodeX, currentNodeY - 1, &adjacentNodeEntries);
    pushNodeAsAlphaIntoVector(currentNodeX, currentNodeY, &adjacentNodeEntries); // We don't want to shoot our own mine.
    pushNodeAsAlphaIntoVector(currentNodeX, currentNodeY + 1, &adjacentNodeEntries);

    pushNodeAsAlphaIntoVector(currentNodeX + 1, currentNodeY - 1, &adjacentNodeEntries);
    pushNodeAsAlphaIntoVector(currentNodeX + 1, currentNodeY, &adjacentNodeEntries);
    pushNodeAsAlphaIntoVector(currentNodeX + 1, currentNodeY + 1, &adjacentNodeEntries);

    return adjacentNodeEntries;
}

void Player::handleMineDetonationLogic(const attemptHitResponse& response) {
    if (hitNode(response.hitNode.node) == HIT_NODE_MINE){
        // The hit grid should update to show a mine has been hit.
        std::vector<adjacentNodeEntry> adjacentNodeEntries = getAdjacentNodes(response.hitNode.x, response.hitNode.y);

        std::set<std::string> uniqueElementsSet{};

        for (auto&& nodeEntry : adjacentNodeEntries){
            attemptPlacementResponse mineExplosionResponse = opposingPlayer->getGameGrid()->attemptPlacement(nodeEntry.letter, nodeEntry.yCoordinate, DESTROYED, VERTICAL);
            battleshipHitGrid.markSuccessfulWarheadStrike(mineExplosionResponse.singleExistingNode.x, mineExplosionResponse.singleExistingNode.y);

            if (hitNode(mineExplosionResponse.singleExistingNode.node) == HIT_NODE_MINE){
                // Recursively call this method to deal with scenarios where a mine explosion affects other mines.
                handleMineDetonationLogic(attemptHitResponse(
                        true,
                        true,
                        attemptPlacementNodeHitResponse(
                                mineExplosionResponse.singleExistingNode.x,
                                mineExplosionResponse.singleExistingNode.y,
                                mineExplosionResponse.singleExistingNode.node)));
            }

            std::string coordinateLetter =
                    convertIncrementingIntegerToAlpha(mineExplosionResponse.singleExistingNode.x + 1)
                    + std::to_string(mineExplosionResponse.singleExistingNode.y + 1);

            for (auto &&ship : opposingPlayer->playerShips){
                if (ship.doesCoordinateIntersectShip(mineExplosionResponse.singleExistingNode.x,
                                                     mineExplosionResponse.singleExistingNode.y)){
                    if (!ship.hasTakenHitFromCoordinate(coordinateLetter)){
                        ship.setTakenHitFromCoordinate(coordinateLetter);
                        ship.setLives(ship.getLives() - 1);
                    }
                }
            }
        }
    }
}

attemptHitResponse Player::executeWarheadStrike(std::string letter, int y) {
    if (y == 0) y++; // We don't like 0's round here.

    attemptPlacementResponse playerHitBoardResponse = getHitGrid()->checkIfNodeExists(letter, y);
    if (playerHitBoardResponse.singleExistingNode.node == VALID_HIT || playerHitBoardResponse.singleExistingNode.node == INVALID_HIT){
        return attemptHitResponse(false, "Cannot fire twice at the same node");
    }

    attemptHitResponse response = opposingPlayer->battleshipGameGrid.receiveWarheadStrike(letter, y);

    if (response.validAttempt){
        if (hitNode(response.hitNode.node) == HIT_NODE_MINE){
            handleMineDetonationLogic(response);
        } else if (response.didHitTarget){
            battleshipHitGrid.markSuccessfulWarheadStrike(response.hitNode.x, response.hitNode.y);
            opposingPlayer->getGameGrid()->attemptPlacement(letter, y, DESTROYED, VERTICAL);
            for (auto &&ship : opposingPlayer->playerShips){
                 std::string coordinateNotation = convertIncrementingIntegerToAlpha(response.hitNode.x + 1)
                                                  + std::to_string(response.hitNode.y + 1);
                 if (ship.doesCoordinateIntersectShip(response.hitNode.x, response.hitNode.y) && !ship.hasTakenHitFromCoordinate(coordinateNotation)){
                     ship
                        .setLives(ship.getLives() - 1)
                        .setTakenHitFromCoordinate(coordinateNotation);
                 }
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

int Player::getNumberOfOperationalShips(){
    int operationalShips = 0;
    for (auto &&ship : playerShips){
        if (ship.isDeployed() && !ship.isSunk()){
            operationalShips++;
        }
    }

    return operationalShips;
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
    if (getNumberOfOperationalShips() == getPlayerShips()->size()){
        return;
    }

    renderPlayerUserInterface();

    int c = 0;
    for (auto &&ship : playerShips){
        if (!ship.isDeployed()){
            deployShipInterface(c);
        }
        c++;
    }

    hasDeployedShip = true;

    if (shouldShowContinueGameConfirmationDialog()) {
        displayContinueGameConfirmationDialogWithPrompt(gameFlowController, "You have placed all your ships. Press enter to continue \033[1;33m[Q to quit or R to reset]:\033[0m ");

        // If we are playing against another human
        clearConsole();
        if (!opposingPlayer->isComputer){
            if (!opposingPlayer->hasDeployedShip){
                getStringWithPrompt(
                        "Please now pass the device to the player: \033[1;31m" + opposingPlayer->playerName
                        + "\033[0m - and then press \033[1;33mENTER\033[0m to start deploying their ships");
            } else {
                getStringWithPrompt(
                        "Please now pass the device to the player: \033[1;31m" + opposingPlayer->playerName
                        + "\033[0m - and then press \033[1;33mENTER\033[0m to start their turn");
            }
        }
        clearConsole();
    }
}

bool Player::deployShipInterface(int shipVertexPosition){
    attemptPlacementResponse response;
    Ship ship = playerShips.at(shipVertexPosition);
    bool hasDisplayedTip = false;

    while (!response.success){
        if (!hasDisplayedTip){
            displayInformation("[TIP]: Leave the input blank to automatically place your "
                               + ship.getName() + " or type AUTO to automatically place the rest of your ships\n");
            hasDisplayedTip = true;
        }

        regexMatch coordinateInput = getRegexInputWithPromptAsRegex(
                "Enter where you want to move your " + ship.getName() + " (eg. A1 or H8): ",
                std::regex("([A-a-Z-z](?:[A-a-B-b])?)([1-9](?:[1-10])?)|!?([Aa][Uu][Tt][Oo])|^\\s*$"));

        if (coordinateInput.match.empty()){
            deployWarshipAutomatically(shipVertexPosition);
            renderPlayerUserInterface();
            return true;
        } else if (convertToUpperCase(coordinateInput.match) == "AUTO") {
            deployWarshipsAutomatically();
            renderPlayerUserInterface();
            return true;
        } else {
            std::string orientation = getRegexInputWithPromptAsString(
                    "Enter ship orientation (vertical/horizontal/v/h): ",
                    std::regex("(\\b[Vv][Ee][Rr][Tt][Ii][Cc][Aa][Ll]|[Hh][Oo][Rr][Ii][Zz][Oo][Nn][Tt][Aa][Ll]|[Vv]\\b|[Hh]\\b)"));

            int yCoordinate = stoi(coordinateInput.matches[1]);

            response = deployShip(shipVertexPosition, convertToUpperCase(coordinateInput.matches[0]),
                                  yCoordinate, convertToUpperCase(orientation).at(0) == 'V' ? VERTICAL : HORIZONTAL);
        }

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

    playerBattleshipGameTable.column(0).format().width(getGameGrid()->getObservableGridWidth());
    playerBattleshipGameTable.column(1).format().width(getGameGrid()->getObservableGridWidth());

    playerBattleshipGameTable.add_row({getGameGrid()->renderGrid(), getHitGrid()->renderGrid()});

    if (alsoRenderComputerBoard){
        playerBattleshipGameTable.add_row({opposingPlayer->playerName + "'s Game board", opposingPlayer->playerName + "'s Hit board"});
        playerBattleshipGameTable.add_row({opposingPlayer->getGameGrid()->renderGrid(), opposingPlayer->getHitGrid()->renderGrid()});
    }

    std::cout << playerBattleshipGameTable << std::endl;
}

tabulate::Table Player::getPlayerShipStatisticsBoard() {
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

    return playerStatisticsBoard;
}

std::string recolourLine(std::string inputLine){
    return replaceStringOccurrencesFromVector(std::move(inputLine),
                                              {"-", "|", "⋅"},
                                              {"\033[37m-\033[0m", "\033[37m|\033[0m", "\033[31m⋅\033[0m"});
}

void Player::renderStatisticsBoard() {
    int interpunctSymbolWidth = 3;
    int numberOfInterpuncts = 4;
    int genericShipBoardMessageLength = 22;
    std::string paddingBetweenBoard = "  ";

    if (alsoRenderComputerBoard){
        std::istringstream humanPlayerBoard(getPlayerShipStatisticsBoard().str());
        std::istringstream computerPlayerBoard(opposingPlayer->getPlayerShipStatisticsBoard().str());

        std::string firstLineCache;
        std::istringstream computerPlayerBoardCache(opposingPlayer->getPlayerShipStatisticsBoard().str());
        std::getline(computerPlayerBoardCache, firstLineCache);

        int padding = firstLineCache.size() - (numberOfInterpuncts * interpunctSymbolWidth) + numberOfInterpuncts -
                genericShipBoardMessageLength - playerName.length();

        std::cout << "\033[1;31m⋅ " << playerName << "'s ship status board\033[0m";
        std::cout << std::string(padding, ' ') << paddingBetweenBoard;
        std::cout << "\033[1;31m⋅ " << opposingPlayer->playerName << "'s ship status board\033[0m" << std::endl;

        for (std::string humanBoardLine; std::getline(humanPlayerBoard, humanBoardLine); ){
            std::string computerBoardLine;
            std::getline(computerPlayerBoard, computerBoardLine);
            std::cout << recolourLine(humanBoardLine) << "  " << recolourLine(computerBoardLine) << std::endl;
        }
    } else {
        std::cout << "\033[1;31m⋅ " << playerName << "'s ship status board\033[0m" << std::endl;
        std::cout << getPlayerShipStatisticsBoard() << std::endl;
    }
}

void Player::renderPlayerUserInterface() {
    clearConsole();

    if (shouldRenderLogStatements()){
        renderPlayerGrid();
        renderStatisticsBoard();

        std::cout << std::endl;
    }
}

void Player::renderCachedComputerWarheadDeploymentResponse(const attemptHitResponse& cachedHitResponse, bool isAutomaticAndRepeatedWarheadStrike, int repeatedWarheadStrikeAttempt) {
    if (cachedHitResponse.validAttempt && isComputer && shouldRenderLogStatements()){
        if (cachedHitResponse.didHitTarget){
            displayInformation((isAutomaticAndRepeatedWarheadStrike ? "\033[1;31mStrike " + std::to_string(repeatedWarheadStrikeAttempt) + " - " : "\033[1;31m")
                + playerName + "'s attempt - \033[1;33mSuccessful warhead strike - hit a "
                + "\033[1;31m" + Ship(cachedHitResponse.hitNode.node).getName() + "\033[1;33m "
                + "at \033[1;31m" + convertIncrementingIntegerToAlpha(cachedHitResponse.hitNode.x + 1) + std::to_string(cachedHitResponse.hitNode.y + 1)
                + "\n", 0);
        } else {
            displayInformation(
                    (isAutomaticAndRepeatedWarheadStrike ? "\033[1;31mStrike " + std::to_string(repeatedWarheadStrikeAttempt) + " - " : "\033[1;31m")
                    + playerName + "'s attempt - \033[1;33mUnsuccessful warhead strike - did not hit anything\n", 0);
        }
    }

    if (!isAutomaticAndRepeatedWarheadStrike && shouldShowContinueGameConfirmationDialog()){
        displayContinueGameConfirmationDialog(getGameFlowController());
    }
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
    int gridHeight = getGameGrid()->getGridHeight();
    int gridWidth = getGameGrid()->getGridWidth();

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

void Player::renderSalvoWarheadStrikeInterface(bool isRepeatingInputSequence, int shipsThatHaveFiredValidWarheadStrikes) {
    std::string multipleCoordinatesNotationInput;
    bool hasInitiatedAutomaticWarheadStrike = false;
    int numberOfOperationalShips = getNumberOfOperationalShips() - shipsThatHaveFiredValidWarheadStrikes;

    displayInformation("[TIP]: Leave the input blank or type AUTO to automatically fire all your warheads\n");

    if (isRepeatingInputSequence){
        multipleCoordinatesNotationInput = getStringWithPrompt(
                "Enter an equal number of coordinates to fire upon (eg. A1 C2 F3): ");
    } else {
        multipleCoordinatesNotationInput = getStringWithPrompt(
                "You have " + std::to_string(numberOfOperationalShips)
                + " remaining warhead strike" + (numberOfOperationalShips > 1 ? "s" : "")
                + ", enter an equal number of coordinates to fire upon (eg. A1 C2 F3): ");
    }

    std::vector<std::string> coordinateNotationInputVector;
    std::vector<std::pair<std::string, int>> letterToIntCoordinateNotation {};
    std::vector<std::pair<std::string, attemptHitResponse>> coordinateNotationToHitResponses {};

    if (multipleCoordinatesNotationInput.empty() || convertToUpperCase(multipleCoordinatesNotationInput) == "AUTO") {
        for (int i = 0; i < numberOfOperationalShips; i++){
            attemptHitResponse response = deployWarheadStrikeAutomatically();
            coordinateNotationToHitResponses.emplace_back(std::pair<std::string, attemptHitResponse>(
                    convertIncrementingIntegerToAlpha(response.hitNode.x + 1) + std::to_string(response.hitNode.y + 1),
                    response));
        }
        hasInitiatedAutomaticWarheadStrike = true;
    } else {
        coordinateNotationInputVector = splitAtCharacterIntoVector(multipleCoordinatesNotationInput, ' ');

        if (coordinateNotationInputVector.size() != numberOfOperationalShips){
            displayError(
                    "You have not entered an equal number of coordinates to operational ships - "
                    + std::to_string(numberOfOperationalShips)
                    + " remaining warhead strike" + (numberOfOperationalShips > 1 ? "s" : "") + "\n",
                    isRepeatingInputSequence ? 3 : 2);
            return renderSalvoWarheadStrikeInterface(true, shipsThatHaveFiredValidWarheadStrikes);
        }

        for (auto&& notation: coordinateNotationInputVector){
            if (!regex_match(notation, BATTLESHIP_INPUT_NOTATION)){
                displayError("Your input: " + notation + " is invalid. Please enter again\n", isRepeatingInputSequence ? 3 : 2);
                return renderSalvoWarheadStrikeInterface(false, shipsThatHaveFiredValidWarheadStrikes);
            }

            regexMatch matchedNotation = getRegexMatchWithString(notation, BATTLESHIP_INPUT_NOTATION);

            letterToIntCoordinateNotation.emplace_back(std::pair<std::string, int>{
                    matchedNotation.matches[0],
                    stoi(matchedNotation.matches[1])
            });
        }

        for (const std::pair<std::string, int>& notation: letterToIntCoordinateNotation){
            coordinateNotationToHitResponses.emplace_back(std::pair<std::string, attemptHitResponse>{
                    convertToUpperCase(notation.first) + std::to_string(notation.second),
                    executeWarheadStrike(convertToUpperCase(notation.first), notation.second)
            });
        }
    }

    int counter = 1;
    renderPlayerUserInterface();
    for (const auto& coordinateNotationToHitResponse: coordinateNotationToHitResponses){
        if (coordinateNotationToHitResponse.second.validAttempt){
            shipsThatHaveFiredValidWarheadStrikes ++;
            if (coordinateNotationToHitResponse.second.didHitTarget && shouldRenderLogStatements()){
                displayInformation(
                        "\033[1;31mStrike " + std::to_string(counter) + ":\033[1;33m "
                        + "Successful warhead strike - hit a \033[1;31m" + Ship(coordinateNotationToHitResponse.second.hitNode.node).getName()
                        + "\033[1;33m at: \033[1;31m" + coordinateNotationToHitResponse.first + "\033[0m\n", 0);
            } else {
                if (shouldRenderLogStatements()){
                    displayInformation(
                            "\033[1;31mStrike " + std::to_string(counter) + ":\033[1;33m "
                            + "Unsuccessful warhead strike at \033[1;31m"
                            + coordinateNotationToHitResponse.first
                            + "\033[1;33m - did not hit anything\n", 0);
                }
            }
        } else {
            if (shouldRenderLogStatements()) {
                displayInformation(
                        "\033[1;31mStrike " + std::to_string(counter) + ": "
                        + "Invalid \033[1;33mwarhead strike at \033[1;31m"
                        + coordinateNotationToHitResponse.first +
                        +"\033[1;33m - " + coordinateNotationToHitResponse.second.message + "\n", 0);
            }
        }

        counter++;
    }

    if (shouldShowContinueGameConfirmationDialog() && !hasInitiatedAutomaticWarheadStrike && counter != 1){
        displayContinueGameConfirmationDialog(getGameFlowController());
    }

    if (shipsThatHaveFiredValidWarheadStrikes != getNumberOfOperationalShips() && numberOfOperationalShips != 0){
        renderPlayerUserInterface();
        return renderSalvoWarheadStrikeInterface(false, shipsThatHaveFiredValidWarheadStrikes);
    }
}

void Player::renderWarheadStrikeInterface() {
    displayInformation("[TIP]: Leave the input blank or type AUTO to automatically fire your warheads\n");

    regexMatch coordinateInput = getRegexInputWithPromptAsRegex(
            "Enter a coordinate to deploy your warhead strike (eg. A1 or H8): ",
            BATTLESHIP_INPUT_NOTATION);
    attemptHitResponse response;
    bool automaticHitResponse = false;

    if (coordinateInput.match.empty() || convertToUpperCase(coordinateInput.match) == "AUTO"){
        response = deployWarheadStrikeAutomatically(0, false);
        automaticHitResponse = true;
    } else {
        int yCoordinate = stoi(coordinateInput.matches[1]);
        response = executeWarheadStrike(convertToUpperCase(coordinateInput.matches[0]), yCoordinate);
    }

    if (response.validAttempt && shouldRenderLogStatements()){
        // Update the board with the hit
        renderPlayerUserInterface();
        if (automaticHitResponse){
            // The usage of displayError is used because this changes the colour to differentiate between displayInformation
            displayError("Automatic warhead strike - ");
        }
        if (response.didHitTarget){
            displayInformation(
                    "Successful warhead strike - hit a \033[1;31m" + Ship(response.hitNode.node).getName()
                    + "\033[1;33m at: \033[1;31m" + convertIncrementingIntegerToAlpha(response.hitNode.x + 1) + std::to_string(response.hitNode.y + 1) + "\033[0m\n", 0);
        } else {
            displayInformation("Unsuccessful warhead strike at \033[1;31m"
                + convertIncrementingIntegerToAlpha(response.hitNode.x + 1) + std::to_string(response.hitNode.y + 1)
                + "\033[1;33m - did not hit anything\n", 0);
        }
    } else {
        if (shouldRenderLogStatements()) {
            displayError(response.message + ": ", 1);
        }
        return renderWarheadStrikeInterface();
    }

    if (shouldShowContinueGameConfirmationDialog()) {
        displayContinueGameConfirmationDialog(getGameFlowController());

        // If we are playing against another human
        if (!opposingPlayer->isComputer){
            clearConsole();
            getStringWithPrompt(
                    "Please now pass the device to the player: \033[1;31m" + opposingPlayer->playerName
                    + "\033[0m - and then press \033[1;33mENTER\033[0m to start the next turn");
            clearConsole();
        }
    }
}

void Player::pushNodeAsAlphaIntoQueue(int x, int y, ShipHuntCoordinateMetadataNodePosition position, std::deque<ShipHuntCoordinateMetadata> *adjacentNodeEntries){
    if (isOutOfBounds(x,y)) return;
    adjacentNodeEntries->push_back(ShipHuntCoordinateMetadata(position, adjacentNodeEntry(convertIncrementingIntegerToAlpha(x), y)));
}

Ship Player::getIntersectingShip(int x, int y){
    std::string coordinateLetter = convertIncrementingIntegerToAlpha(x + 1) + std::to_string(y + 1);
    for (auto &&ship : opposingPlayer->playerShips){
        if (ship.doesCoordinateIntersectShip(x, y) && !ship.hasTakenHitFromCoordinate(coordinateLetter)){
            return ship;
        }
    }

    throw std::runtime_error("No ship intersects these coordinates");
}

attemptHitResponse Player::executeAutomaticEnhancedAlgorithmWarheadStrike(){
    numberOfAttempts ++;
    if (targetMode == HUNT){
        attemptHitResponse hitResponse;

        while (!hitResponse.validAttempt){
            int randomIndex = randomBetween19937(0, potentialNodes.size());
            nodeEntryCoordinate randomNode = potentialNodes[randomIndex];

            std::string letter = convertToUpperCase(convertIncrementingIntegerToAlpha(randomNode.x));

            hitResponse = executeHuntModeWarheadStrikeEnhancedAlgorithm(letter, randomNode.y);
        }

        return hitResponse;
    } else {
        return executeDestroyModeWarheadStrikeEnhancedAlgorithm();
    }
}

attemptHitResponse Player::executeHuntModeWarheadStrikeEnhancedAlgorithm(const std::string& letter, int y){
    if (targetMode != HUNT){
        // Potentially just delegate to the automatic enhanced algorithm warhead strike function here?
        throw std::runtime_error("The target mode must be set to HUNT if firing with coordinates");
    }

    attemptHitResponse hitRequest = executeWarheadStrike(letter, y);

    if (!hitRequest.validAttempt){
        return hitRequest;
    }

    if (hitNode(hitRequest.hitNode.node) == HIT_NODE_SHIP){
        // We've hit a ship, work out which of the opposing player's ship we've hit
        Ship ship = getIntersectingShip(hitRequest.hitNode.x, hitRequest.hitNode.y);
        int shipId = ship.getId();

        shipCoordinatePosition coordinatePosition = shipCoordinatePosition(
                hitRequest.hitNode.x, hitRequest.hitNode.y,
                convertIncrementingIntegerToAlpha(hitRequest.hitNode.x + 1),
                hitRequest.hitNode.y + 1);

        // So we can lock onto the ship matching the ID, we push it into a queue and will train all futures warhead
        // strikes on this particular ship
        shipHuntIdQueue.push(shipId);

        if (shipHuntCache.find(shipId) == shipHuntCache.end()){
            shipHuntCache.insert({ shipId, ShipHunt({ coordinatePosition }, {}) });
            shipHuntCache.at(shipId).maxHealth = ship.getMaxLives();
            shipHuntCache.at(shipId).currentHealth = ship.getLives();
            shipHuntCache.at(shipId).ignoredCoordinates.push_back(letter + std::to_string(y));
        } else {
            shipHuntCache.at(shipId).shipCoordinates.emplace_back(coordinatePosition);
        }

        targetMode = DESTROY;
    }

    return hitRequest;
}

attemptHitResponse Player::executeDestroyModeWarheadStrikeEnhancedAlgorithm(){
    if (targetMode != DESTROY){
        // Potentially just delegate to the manual enhanced algorithm warhead strike function here?
        throw std::runtime_error("The target mode must be set to DESTROY if calling #executeDestroyModeWarheadStrikeEnhancedAlgorithm");
    }

    if (shipHuntIdQueue.empty()){
        targetMode = HUNT;
        // TODO(slyo): There are no pending ships in the queue, switch back to the HUNT mode
        return executeAutomaticEnhancedAlgorithmWarheadStrike();
    }

    int currentShipId = shipHuntIdQueue.front();

    if (shipHuntCache.at(currentShipId).currentHealth <= 0){
        shipHuntIdQueue.pop();
        return executeDestroyModeWarheadStrikeEnhancedAlgorithm();
    }

    std::vector<shipCoordinatePosition> knownCoordinates = shipHuntCache.at(currentShipId).shipCoordinates;

    if (shipHuntCache.at(currentShipId).potentialCoordinatesQueue.empty()){
        // We don't know whether the ship is vertical or horizontal. We will work this out by training missiles: above,
        // below, left and right of the node we have hit and then performing a heuristic to work out whether the ship is
        // vertical or horizontal.

        int currentNodeX = knownCoordinates.at(0).x + 1;
        int currentNodeY = knownCoordinates.at(0).y + 1;

        pushNodeAsAlphaIntoQueue(currentNodeX, currentNodeY - 1, ABOVE, &shipHuntCache.at(currentShipId).potentialCoordinatesQueue); // Above
        pushNodeAsAlphaIntoQueue(currentNodeX, currentNodeY + 1, BELOW, &shipHuntCache.at(currentShipId).potentialCoordinatesQueue); // Below

        pushNodeAsAlphaIntoQueue(currentNodeX - 1, currentNodeY, LEFT, &shipHuntCache.at(currentShipId).potentialCoordinatesQueue); // Left
        pushNodeAsAlphaIntoQueue(currentNodeX + 1, currentNodeY, RIGHT, &shipHuntCache.at(currentShipId).potentialCoordinatesQueue); // Right

        // We've select a coordinate which represents above, below, left and right and pushed these into our queue
        // we can now recursively call this method again which will incrementally work through the queue
        return executeDestroyModeWarheadStrikeEnhancedAlgorithm();
    }

    // Get the first coordinate in our cache of potential coordinates and store it
    std::deque<ShipHuntCoordinateMetadata> *queue = &shipHuntCache.at(currentShipId).potentialCoordinatesQueue;
    ShipHuntCoordinateMetadata firstCoordinate = queue->front();

    // Fire at the potential coordinate and remove it from the queue
    attemptHitResponse hitResponse = executeWarheadStrike(firstCoordinate.nodeEntry.letter, firstCoordinate.nodeEntry.yCoordinate);
    queue->pop_front();

    if (!hitResponse.validAttempt){
        return executeDestroyModeWarheadStrikeEnhancedAlgorithm();
    }

    if (hitNode(hitResponse.hitNode.node) != HIT_NODE_SHIP){
        std::string coordinateLetter = firstCoordinate.nodeEntry.letter + std::to_string(firstCoordinate.nodeEntry.yCoordinate);
        shipHuntCache.at(currentShipId).ignoredCoordinates.push_back(coordinateLetter);
        return hitResponse;
    }

    Ship ship = getIntersectingShip(hitResponse.hitNode.x, hitResponse.hitNode.y);

    if (ship.getId() != currentShipId){
        // We've hit another ship...lets add that to our map because now we'll be able to target another ship afterwards
        shipCoordinatePosition coordinatePosition = shipCoordinatePosition(
                hitResponse.hitNode.x, hitResponse.hitNode.y,
                convertIncrementingIntegerToAlpha(hitResponse.hitNode.x + 1),
                hitResponse.hitNode.y + 1);

        shipHuntIdQueue.push(ship.getId());
        shipHuntCache.insert({ ship.getId(), ShipHunt({ coordinatePosition }, {}) });

        // We've already popped the offending (and incorrect) coordinate from our queue, hopefully we'll
        // hit the ship we're attempting to hit the next time...
        return hitResponse;
    }

    shipHuntCache.at(currentShipId).currentHealth = ship.getLives();

    if (shipHuntCache.at(currentShipId).shipOrientation == SHIP_HUNT_ORIENTATION_UNKNOWN){
        shipHuntCache.at(currentShipId).potentialCoordinatesQueue.clear();

        auto ignoredCoordinates = shipHuntCache.at(currentShipId).ignoredCoordinates;

        // Great! We've hit the same ship, lets set the orientation
        // We don't yet know the orientation of the ship, so lets set it
        if (firstCoordinate.nodePosition == ABOVE || firstCoordinate.nodePosition == BELOW){
            shipHuntCache.at(currentShipId).shipOrientation = SHIP_HUNT_VERTICAL;

            for (int i = hitResponse.hitNode.y; i > hitResponse.hitNode.y - ship.getMaxLives(); i--){
                int coordinateX = hitResponse.hitNode.x + 1;
                int coordinateY = i;

                if (!isOutOfBounds(coordinateX, coordinateY)
                    && hitResponse.hitNode.y + 1 != coordinateY){
                    std::string coordinateLetter = convertIncrementingIntegerToAlpha(coordinateX) + std::to_string(coordinateY);

                    if (std::find(ignoredCoordinates.begin(), ignoredCoordinates.end(), coordinateLetter) == ignoredCoordinates.end()){
                        pushNodeAsAlphaIntoQueue(coordinateX, coordinateY, PRE, &shipHuntCache.at(currentShipId).potentialCoordinatesQueue);
                    }
                }
            }
            for (int i = hitResponse.hitNode.y + 1; i <= hitResponse.hitNode.y + ship.getMaxLives(); i++){
                int coordinateX = hitResponse.hitNode.x + 1;
                int coordinateY = i + 1;

                if (!isOutOfBounds(coordinateX, coordinateY)) {
                    std::string coordinateLetter = convertIncrementingIntegerToAlpha(coordinateX) + std::to_string(coordinateY);

                    if (std::find(ignoredCoordinates.begin(), ignoredCoordinates.end(), coordinateLetter) == ignoredCoordinates.end()){
                        pushNodeAsAlphaIntoQueue(coordinateX, coordinateY, POST, &shipHuntCache.at(currentShipId).potentialCoordinatesQueue);
                    }
                }
            }
        } else {
            shipHuntCache.at(currentShipId).shipOrientation = SHIP_HUNT_HORIZONTAL;

            for (int i = hitResponse.hitNode.x; i > hitResponse.hitNode.x - ship.getMaxLives(); i--){
                int coordinateX = i;
                int coordinateY = hitResponse.hitNode.y + 1;

                if (!isOutOfBounds(coordinateX, coordinateY)
                    && hitResponse.hitNode.x + 1 != coordinateX){
                    std::string coordinateLetter = convertIncrementingIntegerToAlpha(coordinateX) + std::to_string(coordinateY);

                    if (std::find(ignoredCoordinates.begin(), ignoredCoordinates.end(), coordinateLetter) == ignoredCoordinates.end()){
                        pushNodeAsAlphaIntoQueue(coordinateX, coordinateY, PRE, &shipHuntCache.at(currentShipId).potentialCoordinatesQueue);
                    }
                }
            }

            for (int i = 1; i < ship.getMaxLives() + 1; i++){
                int coordinateX = hitResponse.hitNode.x + 1 + i;
                int coordinateY = hitResponse.hitNode.y + 1;

                if (!isOutOfBounds(coordinateX, coordinateY)) {
                    std::string coordinateLetter = convertIncrementingIntegerToAlpha(coordinateX) + std::to_string(coordinateY);

                    if (std::find(ignoredCoordinates.begin(), ignoredCoordinates.end(), coordinateLetter) == ignoredCoordinates.end()){
                        pushNodeAsAlphaIntoQueue(coordinateX, coordinateY, POST, &shipHuntCache.at(currentShipId).potentialCoordinatesQueue);
                    }
                }
            }
        }
    }

    return hitResponse;
}

attemptHitResponse Player::deployWarheadStrikeAutomatically(int attempts, bool isAutomaticAndRepeatedWarheadStrike) {
    attemptHitResponse hitResponse;

    while (!hitResponse.validAttempt){
        int randomIndex = randomBetween19937(0, potentialNodes.size());
        nodeEntryCoordinate randomNode = potentialNodes[randomIndex];

        std::string letter = convertToUpperCase(convertIncrementingIntegerToAlpha(randomNode.x));
        hitResponse = executeWarheadStrike(letter, randomNode.y);

        if (attempts == MAX_WARHEAD_STRIKES_ATTEMPTS){
            displayError("Max strikes attempted reached, please increase the board size", 0);
            exit (EXIT_FAILURE);
        }

        attempts ++;
    }

    if (isComputer){
        if (!isAutomaticAndRepeatedWarheadStrike){
            opposingPlayer->renderPlayerUserInterface();
            renderCachedComputerWarheadDeploymentResponse(hitResponse);
        }
    }

    numberOfAttempts ++;
    return hitResponse;
}

void Player::deployWarheadStrikesAutomatically() {
    std::vector<attemptHitResponse> warheadStrikeRequests {};
    for (int i = 0; i < getNumberOfOperationalShips(); i++){
        warheadStrikeRequests.emplace_back(deployWarheadStrikeAutomatically(0, true));
    }

    if (isComputer){
        opposingPlayer->renderPlayerUserInterface();
    }

    int counter = 1;
    for (auto&& hitResponse : warheadStrikeRequests){
        renderCachedComputerWarheadDeploymentResponse(hitResponse, true, counter);
        counter ++;
    }

    if (shouldShowContinueGameConfirmationDialog()) {
        displayContinueGameConfirmationDialog(getGameFlowController());
    }

    numberOfAttempts ++;
}

bool Player::hasPlayerLostAllShips() {
    return (std::all_of(playerShips.cbegin(), playerShips.cend(), [](auto&& ship){ return ship.isSunk(); }));
}

GameFlowController * Player::getGameFlowController() const {
    return gameFlowController;
}

void Player::deployMultipleRandomlyPositionedMines() {
    for (int i = 0; i < 5; i++){
        attemptPlacementResponse mineDeploymentResponse;

        while (!mineDeploymentResponse.success){
            int randomXCoordinate = randomBetween19937(0, getGameGrid()->getGridWidth());
            int randomYCoordinate = randomBetween19937(0, getGameGrid()->getGridHeight());
            mineDeploymentResponse = deployMine(randomXCoordinate, randomYCoordinate);
        }
    }
}

bool Player::isComputerPlayingAgainstComputer() {
    return isComputer && opposingPlayer->isComputer;
}

bool Player::shouldShowContinueGameConfirmationDialog() {
    return (!isComputer && !opposingPlayer->isComputer)
    || (!isComputer && opposingPlayer->isComputer) || (isComputer && !opposingPlayer->isComputer)
    || (isComputerPlayingAgainstComputer() && SHOULD_SHOW_CONTINUE_GAME_CONFIRMATION_DIALOG_DURING_AUTOMATION);
}

bool Player::shouldRenderLogStatements() {
    return (!isComputer && !opposingPlayer->isComputer)
    || (!isComputer && opposingPlayer->isComputer)
    || (isComputer && !opposingPlayer->isComputer)
    || (isComputerPlayingAgainstComputer() && SHOULD_SHOW_LOG_STATEMENTS_DURING_AUTOMATION);
}

void Player::setPlayerType(PlayerType type) {
    playerType = type;
    if (type == COMPUTER){
        isComputer = true;
    }
}

void Player::setPlayingAgainstComputer() {
    opposingPlayer->isComputer = true;
    alsoRenderComputerBoard = true;
}

PlayerType Player::getPlayerType() {
    return playerType;
}
