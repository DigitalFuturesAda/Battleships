//
// Created by Suraj Lyons on 01/03/2021.
//

#include "Player.h"
#include <iostream>
#include <set>
#include <utility>
#include <absl/strings/ascii.h>
#include "../../components/grid/GameGrid.h"
#include "../util/strings.h"
#include "../util/io.h"
#include "../util/rand.h"

Player::Player(std::string playerName, GameFlowController &gameFlowController) {
    this->playerShips = {
            Ship(CARRIER, false),
            Ship(BATTLESHIP, false),
            Ship(DESTROYER, false),
            Ship(SUBMARINE, false),
            Ship(PATROL, false),
    };
    this->playerName = std::move(playerName);
    this->gameFlowController = &gameFlowController;

    for (int i = 0; i <= GameGrid::WIDTH; i++){
        for (int v = 0; v <= GameGrid::HEIGHT; v++){
            potentialNodes.emplace_back(i, v);
        }
    }
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

bool hitMine(GridNodes hitNode){
    switch (hitNode){
        case EMPTY:
        case UNKNOWN:
        case DESTROYED:
        case VALID_HIT:
        case INVALID_HIT:
        case CARRIER:
        case BATTLESHIP:
        case DESTROYER:
        case SUBMARINE:
        case PATROL:
            return false;
        case MINE:
        case CARRIER_MINE:
        case BATTLESHIP_MINE:
        case DESTROYER_MINE:
        case SUBMARINE_MINE:
        case PATROL_MINE:
            return true;
    }
}

void pushNodeAsAlphaIntoVector(int x, int y, std::vector<adjacentNodeEntry> *adjacentNodeEntries){
    if (x < 1 || y < 1 || y > GameGrid::HEIGHT || x > GameGrid::WIDTH) return;
    adjacentNodeEntries->push_back(adjacentNodeEntry(convertIncrementingIntegerToAlpha(x), y));
}

std::vector<adjacentNodeEntry> getAdjacentNodes(int x, int y){
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
    if (hitMine(response.hitNode.node)){
        // The hit grid should update to show a mine has been hit.
        std::vector<adjacentNodeEntry> adjacentNodeEntries = getAdjacentNodes(response.hitNode.x, response.hitNode.y);

        std::set<std::string> uniqueElementsSet{};

        for (auto&& nodeEntry : adjacentNodeEntries){
            attemptPlacementResponse mineExplosionResponse = opposingPlayer->getGameGrid()->attemptPlacement(nodeEntry.letter, nodeEntry.yCoordinate, DESTROYED, VERTICAL);
            battleshipHitGrid.markSuccessfulWarheadStrike(mineExplosionResponse.singleExistingNode.x, mineExplosionResponse.singleExistingNode.y);

            if (hitMine(mineExplosionResponse.singleExistingNode.node)){
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
        if (hitMine(response.hitNode.node)){
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

    displayContinueGameConfirmationDialogWithPrompt(gameFlowController, "You have placed all your ships. Press enter to continue \033[1;33m[Q to quit or R to reset]:\033[0m ");
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
                std::regex("([A-a-Z-z](?:[A-a-B-b])?)([1-9](?:[1-10])?)"));

        std::string orientation = getRegexInputWithPromptAsString(
                "Enter ship orientation (vertical/horizontal/v/h): ",
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

void Player::setPlayingAgainstComputer() {
    opposingPlayer->isComputer = true;
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

void Player::renderSalvoWarheadStrikeInterface(bool isRepeatingInputSequence, int shipsThatHaveFiredValidWarheadStrikes) {
    std::string multipleCoordinatesNotationInput;
    int numberOfOperationalShips = getNumberOfOperationalShips() - shipsThatHaveFiredValidWarheadStrikes;

    if (isRepeatingInputSequence){
        multipleCoordinatesNotationInput = getStringWithPrompt(
                "Enter an equal number of coordinates to fire upon (eg. A1 C2 F3): ");
    } else {
        multipleCoordinatesNotationInput = getStringWithPrompt(
                "You have " + std::to_string(numberOfOperationalShips)
                + " remaining warhead strike" + (numberOfOperationalShips > 1 ? "s" : "")
                + ", enter an equal number of coordinates to fire upon (eg. A1 C2 F3): ");
    }

    std::vector<std::string> coordinateNotationInputVector = splitAtCharacterIntoVector(multipleCoordinatesNotationInput, ' ');
    std::vector<std::pair<std::string, int>> letterToIntCoordinateNotation {};
    std::vector<std::pair<std::string, attemptHitResponse>> coordinateNotationToHitResponses {};


    if (coordinateNotationInputVector.size() != numberOfOperationalShips){
        displayError(
                "You have not entered an equal number of coordinates to operational ships - "
                + std::to_string(numberOfOperationalShips)
                + " remaining warhead strike" + (numberOfOperationalShips > 1 ? "s" : "") + "\n",
                isRepeatingInputSequence ? 2 : 1);
        return renderSalvoWarheadStrikeInterface(true, shipsThatHaveFiredValidWarheadStrikes);
    }

    for (auto&& notation: coordinateNotationInputVector){
        if (!regex_match(notation, BATTLESHIP_INPUT_NOTATION)){
            displayError("Your input: " + notation + " is invalid. Please enter again\n", isRepeatingInputSequence ? 2 : 1);
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

    if (shouldShowContinueGameConfirmationDialog()){
        displayContinueGameConfirmationDialog(getGameFlowController());
    }

    if (shipsThatHaveFiredValidWarheadStrikes != getNumberOfOperationalShips() && numberOfOperationalShips != 0){
        renderPlayerUserInterface();
        return renderSalvoWarheadStrikeInterface(false, shipsThatHaveFiredValidWarheadStrikes);
    }
}

void Player::renderWarheadStrikeInterface() {
    regexMatch coordinateInput = getRegexInputWithPromptAsRegex(
            "Enter a coordinate to deploy your warhead strike (eg. A1 or H8): ",
            BATTLESHIP_INPUT_NOTATION);

    int yCoordinate = stoi(coordinateInput.matches[1]);
    attemptHitResponse response = executeWarheadStrike(convertToUpperCase(coordinateInput.matches[0]), yCoordinate);

    if (response.validAttempt && shouldRenderLogStatements()){
        // Update the board with the hit
        renderPlayerUserInterface();
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
    }
}

attemptHitResponse Player::deployWarheadStrikeAutomatically(int attempts, bool isAutomaticAndRepeatedWarheadStrike) {
    // TODO(slyo): Verify all ships not destroyed. Maybe for the purposes of this game we can assume the computer has
    //  rudimentary knowledge of the approx bounds of players ships location as to decrease trial and error.
    //  Potentially once n% of the board has been destroyed, we randomly guess from a vector of coordinates which have
    //  not been fired upon. Taking this further we could actually store a cache of coords which have not been destroyed
    //  which removes the retry logic entirely.

    int gridHeight = GameGrid::HEIGHT;
    int gridWidth = GameGrid::WIDTH;

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
            int randomXCoordinate = randomBetween19937(0, GameGrid::WIDTH);
            int randomYCoordinate = randomBetween19937(0, GameGrid::HEIGHT);
            mineDeploymentResponse = deployMine(randomXCoordinate, randomYCoordinate);
        }
    }
}

bool Player::isComputerPlayingAgainstComputer() {
    return isComputer && opposingPlayer->isComputer;
}

bool Player::shouldShowContinueGameConfirmationDialog() {
    return (!isComputer && opposingPlayer->isComputer) || (isComputer && !opposingPlayer->isComputer) ||
        (isComputerPlayingAgainstComputer() && SHOULD_SHOW_CONTINUE_GAME_CONFIRMATION_DIALOG_DURING_AUTOMATION);
}

bool Player::shouldRenderLogStatements() {
    return (!isComputer && opposingPlayer->isComputer) || (isComputer && !opposingPlayer->isComputer) ||
           (isComputerPlayingAgainstComputer() && SHOULD_SHOW_LOG_STATEMENTS_DURING_AUTOMATION);
}