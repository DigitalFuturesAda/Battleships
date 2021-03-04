//
// Created by Suraj Lyons on 01/03/2021.
//

#include "GameGrid.h"
#include "../util/strings.h"
#include <iostream>
#include <utility>
#include <absl/strings/str_format.h>
#include <absl/strings/ascii.h>

std::string GameGrid::renderGrid() {
    int verticalCounter = 1;
    int horizontalPadding = std::to_string(HEIGHT).length();

    std::ostringstream stringStream;

    stringStream << "X";

    stringStream << std::string(horizontalPadding, ' ');
    for (int i = 1; i <= WIDTH; i ++){
        std::string horizontalText = " " + convertIncrementingIntegerToAlpha(i) + " ";
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
    return (WIDTH * 3) + 5;
}

std::string GameGrid::formatNode(GridNodes node) {
    switch (node) {
        case EMPTY:
            return "\033[1;37m[ ]\033[0m";
        case DESTROYED:
            return "\033[1;30m[■]\033[0m";
        case MINE:
            return "\033[1;34m[◘]\033[0m";
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
        case VALID_HIT:
            return "\033[1;31m[■]\033[0m";
        case INVALID_HIT:
            return "\033[1;37m[■]\033[0m";
        case UNKNOWN:
            return "\033[1;37m<?>\033[0m";
    }
}

int GameGrid::getEntityConstraints(GridNodes placeableNodes){
    switch (placeableNodes) {
        case EMPTY:
        case DESTROYED:
        case VALID_HIT:
        case INVALID_HIT:
        case MINE:
            return 1;
        case CARRIER:
            return 5;
        case BATTLESHIP:
            return 4;
        case DESTROYER:
        case SUBMARINE:
            return 3;
        case PATROL:
            return 2;
        case UNKNOWN:
            throw std::runtime_error("GameGrid#getEntityConstraints canot be called with an UNKNOWN GridNode");
    }
}

attemptPlacementResponse GameGrid::attemptPlacement(int x, int y, GridNodes node, Orientation orientation) {
    int entityConstraints = getEntityConstraints(node);

    // Ensure x,y coords are within the confines of the grid.
    if (y > HEIGHT || x > WIDTH){
        return attemptPlacementResponse(false, "xy coordinates not within the confines of the grid");
    }

    // Ensure we're not trying to place the starting node on a taken tile unless we're placing an empty node.
    GridNodes existingNode = battleshipGameGrid[y][x];
    if (existingNode != EMPTY && node != DESTROYED){
        return attemptPlacementResponse(false, "Can not place starting node in non-empty tile");
    }

    if (orientation == VERTICAL){
        if (y + entityConstraints > HEIGHT){
            return attemptPlacementResponse(false, "Entity height exceeds grid dimensions");
        }

        for (int i = y; i <= y + entityConstraints - 1; i++){
            GridNodes potentialNode = battleshipGameGrid[i][x];
            if (potentialNode != EMPTY && node != DESTROYED){
                return attemptPlacementResponse(false, "Can not place node in non-empty tile");
            }
        }

        for (int i = y; i <= y + entityConstraints - 1; i++){
            battleshipGameGrid[i][x] = node;
        }
    } else {
        if (x + entityConstraints > WIDTH){
            return attemptPlacementResponse(false, "Entity width exceeds grid dimensions");
        }

        for (int i = x; i <= x + entityConstraints - 1; i++){
            GridNodes potentialNode = battleshipGameGrid[y][i];
            if (potentialNode != EMPTY && node != EMPTY){ // TODO(slyo): Access whether overwriting is allowed.
                return attemptPlacementResponse(false, "Can not place node in non-empty tile");
            }
        }

        for (int i = x; i <= x + entityConstraints - 1; i++){
            battleshipGameGrid[y][i] = node;
        }
    }

    attemptPlacementResponse response = attemptPlacementResponse(true);
    response.existingNode = attemptPlacementNodeHitResponse(existingNode);
    // This logic allows us to return a response which includes the node which has been replaced, this is useful if we
    // have just fired a torpedo.
    return response;
}

attemptPlacementResponse GameGrid::attemptPlacement(std::string letter, int number, GridNodes node, Orientation orientation) {
    return attemptPlacement(convertAlphaToIncrementingInteger(std::move(letter)) - 1, number - 1, node, orientation);
}

attemptPlacementResponse GameGrid::checkIfNodeExists(std::string letter, int number) {
    int x = convertAlphaToIncrementingInteger(std::move(letter)) - 1;
    // Arrays start at 0
    int y = number - 2;

    if (y > HEIGHT || x > WIDTH){
        return attemptPlacementResponse(false, "xy coordinates not within the confines of the grid");
    }

    GridNodes existingNode = battleshipGameGrid[y][x];
    return attemptPlacementResponse(true, attemptPlacementNodeHitResponse(x, y, existingNode));
}

attemptHitResponse GameGrid::receiveWarheadStrike(std::string letter, int number) {
    attemptPlacementResponse hitResponse = this->checkIfNodeExists(std::move(letter), number);

    if (hitResponse.success){
        if (hitResponse.existingNode.node == DESTROYED){
            return attemptHitResponse(false, false, hitResponse.existingNode, "Cannot fire at a destroyed tile");
        } else if (hitResponse.existingNode.node == EMPTY){
            return attemptHitResponse(true, false, hitResponse.existingNode);
        }
        return attemptHitResponse(true, true, hitResponse.existingNode);
    }

    return attemptHitResponse(false, hitResponse.message);
}


GameGrid::GameGrid() {
    for ( auto & v : battleshipGameGrid )
        for ( auto & e : v )
            e = EMPTY;
};
