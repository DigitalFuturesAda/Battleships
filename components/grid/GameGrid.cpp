//
// Created by Suraj Lyons on 01/03/2021.
//

#include "GameGrid.h"
#include "../util/strings.h"
#include "../config/ConfigSingleton.h"
#include "../ship/Ship.h"
#include <iostream>
#include <utility>
#include <absl/strings/str_format.h>
#include <absl/strings/ascii.h>

std::string GameGrid::renderGrid() {
    int verticalCounter = 1;
    int horizontalPadding = std::to_string(getGridHeight()).length();

    std::ostringstream stringStream;

    if (getGridWidth() > 26){
        stringStream << "*";
        stringStream << std::string(horizontalPadding, ' ');
        for (int i = 1; i <= getGridWidth(); i ++){
            std::string horizontalText;

            if (i < 27){
                horizontalText = "   ";
            } else {
                horizontalText = " " + std::string(1, convertIncrementingIntegerToAlpha(i).at(0)) + " ";
            }

            stringStream << horizontalText;
        }

        stringStream << std::endl;
    }

    stringStream << "~";
    stringStream << std::string(horizontalPadding, ' ');
    for (int i = 1; i <= getGridWidth(); i ++){
        std::string horizontalText;

        if (i > 26){
            horizontalText = " " + std::string(1, convertIncrementingIntegerToAlpha(i).at(1)) + " ";
        } else {
            horizontalText = " " + convertIncrementingIntegerToAlpha(i) + " ";
        }

        stringStream << horizontalText;
    }

    stringStream << std::endl;

    for (const auto& gridRow : battleshipGameGrid){
        stringStream << absl::StrFormat("%-2d", verticalCounter) << " ";
        for (const GridNodes node : gridRow){
            stringStream << formatNode(node) << "";
        }
        stringStream << std::endl;

        verticalCounter ++;
    }

    return stringStream.str();
}

int GameGrid::getObservableGridWidth() {
    return (getGridWidth() * 3) + 5;
}

std::string GameGrid::formatNode(GridNodes node) {
    switch (node) {
        case UNKNOWN:
            return "\033[1;37m<?>\033[0m";
        case EMPTY:
            return "\033[1;37m[ ]\033[0m";
        case DESTROYED:
            return "\033[1;30m[■]\033[0m";

        case MINE:
            return "\033[1;34m[◘]\033[0m";

        case VALID_HIT:
            return "\033[1;31m[■]\033[0m";
        case INVALID_HIT:
            return "\033[1;37m[■]\033[0m";

        case CARRIER:
            return "\033[1;31m C \033[0m";
        case BATTLESHIP:
            return "\033[1;31m B \033[0m";
        case DESTROYER:
            return "\033[1;31m D \033[0m";
        case SUBMARINE:
            return "\033[1;31m S \033[0m";
        case PATROL:
            return "\033[1;31m P \033[0m";

        case CARRIER_MINE:
            return "\033[1;34m[C]\033[0m";
        case BATTLESHIP_MINE:
            return "\033[1;34m[B]\033[0m";
        case DESTROYER_MINE:
            return "\033[1;34m[D]\033[0m";
        case SUBMARINE_MINE:
            return "\033[1;34m[S]\033[0m";
        case PATROL_MINE:
            return "\033[1;34m[P]\033[0m";
    }
}

int GameGrid::getEntityConstraints(GridNodes placeableNodes){ // NOLINT(misc-no-recursion) - max 1 recursion.
    switch (placeableNodes) {
        case EMPTY:
        case DESTROYED:
        case VALID_HIT:
        case INVALID_HIT:
        case MINE:
            return 1;

        case CARRIER:
        case BATTLESHIP:
        case DESTROYER:
        case SUBMARINE:
        case PATROL:
            return ConfigSingleton::getInstance()->getValidator().getShipHealthMap().at(Ship::getShipName(placeableNodes));

        case CARRIER_MINE:
            return getEntityConstraints(CARRIER);
        case BATTLESHIP_MINE:
            return getEntityConstraints(BATTLESHIP);
        case DESTROYER_MINE:
            return getEntityConstraints(DESTROYER);
        case SUBMARINE_MINE:
            return getEntityConstraints(SUBMARINE);
        case PATROL_MINE:
            return getEntityConstraints(PATROL);

        case UNKNOWN:
            throw std::runtime_error("GameGrid#getEntityConstraints cannot be called with an UNKNOWN GridNode");
    }
}

GridNodes convertShipToMineWithShip(GridNodes currentShip){
    switch (currentShip){
        case EMPTY:
        case DESTROYED:
        case VALID_HIT:
        case INVALID_HIT:
        case MINE:
        case UNKNOWN:
            return MINE;
        case CARRIER:
        case CARRIER_MINE:
            return CARRIER_MINE;
        case BATTLESHIP:
        case BATTLESHIP_MINE:
            return BATTLESHIP_MINE;
        case DESTROYER:
        case DESTROYER_MINE:
            return DESTROYER_MINE;
        case SUBMARINE:
        case SUBMARINE_MINE:
            return SUBMARINE_MINE;
        case PATROL:
        case PATROL_MINE:
            return PATROL_MINE;
    }
}

attemptPlacementResponse validateAttemptPlacement(GridNodes existingNode, GridNodes placementNode){
    if (existingNode != EMPTY && !(placementNode == DESTROYED || placementNode == MINE)){
        return attemptPlacementResponse(false, "Can not place node in non-empty tile");
    }
    if (placementNode == MINE && (existingNode == UNKNOWN || existingNode == DESTROYED || existingNode == MINE)){
        return attemptPlacementResponse(false, "Can not place a mine on a restricted tile");
    }

    return attemptPlacementResponse(true);
}

attemptPlacementResponse GameGrid::attemptPlacement(int x, int y, GridNodes node, Orientation orientation) {
    int entityConstraints = getEntityConstraints(node);
    std::vector<shipCoordinatePosition> shipCoordinatePositions;

    // Ensure x,y coords are within the confines of the grid.
    if (y >= getGridHeight() || x >= getGridWidth() || y < 0 || x < 0){
        return attemptPlacementResponse(false, "xy coordinates not within the confines of the grid");
    }

    // Ensure we're not trying to place the starting node on a taken tile unless we're placing an empty node.
    GridNodes existingNode = battleshipGameGrid[y][x];
    attemptPlacementResponse placementRequest = validateAttemptPlacement(existingNode, node);
    if (!placementRequest.success){
        return placementRequest;
    }

    if (orientation == VERTICAL){
        if (y + entityConstraints > getGridHeight()){
            return attemptPlacementResponse(false, "Entity height exceeds grid dimensions");
        }

        for (int i = y; i <= y + entityConstraints - 1; i++){
            GridNodes potentialNode = battleshipGameGrid[i][x];
            attemptPlacementResponse placementRequestWithPotentialNode = validateAttemptPlacement(potentialNode, node);
            if (!placementRequestWithPotentialNode.success){
                return placementRequestWithPotentialNode;
            }
        }

        for (int i = y; i <= y + entityConstraints - 1; i++){
            if (node == MINE){
                battleshipGameGrid[i][x] = convertShipToMineWithShip(battleshipGameGrid[i][x]);
            } else {
                battleshipGameGrid[i][x] = node;
            }
            shipCoordinatePositions.emplace_back(
                    /* x = */ x, /* y = */ i,
                    convertIncrementingIntegerToAlpha(x + 1), i + 1);
        }
    } else {
        if (x + entityConstraints > getGridWidth()){
            return attemptPlacementResponse(false, "Entity width exceeds grid dimensions");
        }

        for (int i = x; i <= x + entityConstraints - 1; i++){
            GridNodes potentialNode = battleshipGameGrid[y][i];
            attemptPlacementResponse placementRequestWithPotentialNode = validateAttemptPlacement(potentialNode, node);
            if (!placementRequestWithPotentialNode.success){
                return placementRequestWithPotentialNode;
            }
        }

        for (int i = x; i <= x + entityConstraints - 1; i++){
            battleshipGameGrid[y][i] = node;
            shipCoordinatePositions.emplace_back(
                    /* x = */ i, /* y = */ y,
                    convertIncrementingIntegerToAlpha(i + 1), y + 1);
        }
    }

    if (entityConstraints == 1){
        // This logic allows us to return a response which includes the node which has been replaced, this is useful if we
        // have just fired a torpedo.
        attemptPlacementResponse response = attemptPlacementResponse(true);
        response.singleExistingNode = attemptPlacementNodeHitResponse(x, y, existingNode);

        return response;
    }

    return attemptPlacementResponse(true, shipCoordinatePositions);
}

attemptPlacementResponse GameGrid::attemptPlacement(std::string letter, int number, GridNodes node, Orientation orientation) {
    return attemptPlacement(convertAlphaToIncrementingInteger(std::move(letter)) - 1, number - 1, node, orientation);
}

attemptPlacementResponse GameGrid::checkIfNodeExists(std::string letter, int number) {
    int x = convertAlphaToIncrementingInteger(std::move(letter)) - 1;
    // Arrays start at 0
    int y = number - 1;

    if (y > getGridHeight() - 1|| x > getGridWidth()){
        return attemptPlacementResponse(false, "xy coordinates not within the confines of the grid");
    }

    GridNodes existingNode = battleshipGameGrid[y][x];
    return attemptPlacementResponse(true, attemptPlacementNodeHitResponse(x, y, existingNode));
}

attemptHitResponse GameGrid::receiveWarheadStrike(std::string letter, int number) {
    attemptPlacementResponse hitResponse = this->checkIfNodeExists(std::move(letter), number);

    if (hitResponse.success){
        if (hitResponse.singleExistingNode.node == DESTROYED){
            return attemptHitResponse(false, false, hitResponse.singleExistingNode, "Cannot fire at a destroyed tile");
        } else if (hitResponse.singleExistingNode.node == EMPTY){
            return attemptHitResponse(true, false, hitResponse.singleExistingNode);
        }
        return attemptHitResponse(true, true, hitResponse.singleExistingNode);
    }

    return attemptHitResponse(false, hitResponse.message);
}

GameGrid::GameGrid() {
    configBoardDimensions dimensions = ConfigSingleton::getInstance()->getValidator().getBoardDimensions();
    gridHeight = dimensions.height;
    gridWidth = dimensions.width;

    for (int i = 0; i < gridHeight; i++){
        battleshipGameGrid.push_back(std::vector<GridNodes>(gridWidth));
        for (int x = 0; x < gridWidth; x++){
            battleshipGameGrid[i][x] = EMPTY;
        }
    }
}

int GameGrid::getGridHeight() const {
    return gridHeight;
}

int GameGrid::getGridWidth() const {
    return gridWidth;
};
