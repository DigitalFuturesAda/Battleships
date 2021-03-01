//
// Created by Suraj Lyons on 01/03/2021.
//

#include "GameGrid.h"
#include "../util/strings.h"
#include <iostream>
#include <absl/strings/str_format.h>
#include <absl/strings/ascii.h>

std::string GameGrid::getGrid() {
    int verticalCounter = 1;
    int horizontalPadding = std::to_string(HEIGHT).length() + 1;

    std::ostringstream stringStream;

    stringStream << std::string(horizontalPadding, ' ');
    for (int i = 1; i <= WIDTH; i ++){
        stringStream << " " << convertIncrementingIntegerToAlpha(i) << " ";
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

std::string GameGrid::formatNode(GridNodes node) {
    switch (node) {
        case EMPTY:
            return "\033[1;37m[ ]\033[0m";
        case DESTROYED:
            return "\033[1;30m[■]\033[0m";
        case MINE:
            return " \033[1;36mM\033[0m ";
        case CARRIER:
            return " \033[1;31mC\033[0m ";
        case BATTLESHIP:
            return " \033[1;31mB\033[0m ";
        case DESTROYER:
            return " \033[1;31mD\033[0m ";
        case SUBMARINE:
            return " \033[1;31mS\033[0m ";
        case PATROL:
            return " \033[1;31mP\033[0m ";
    }
}

int GameGrid::getEntityConstraints(GridNodes placeableNodes){
    switch (placeableNodes) {
        case EMPTY:
            throw std::range_error("Invalid node.");
        case DESTROYED:
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
    }
}

attemptPlacementResponse GameGrid::attemptPlacement(int x, int y, GridNodes node, Orientation orientation) {
    int entityConstraints = getEntityConstraints(node);

    // Ensure x,y coords are within the confines of the grid.
    if (y > HEIGHT || x > WIDTH){
        return attemptPlacementResponse(false, "xy coordinates not within the confines of the grid");
    }

    // Ensure we're not trying to place the starting node on a taken tile.
    GridNodes existingNode = battleshipGameGrid[y][x];
    if (existingNode != EMPTY){
        return attemptPlacementResponse(false, "Can not place starting node in non-empty tile");
    }

    if (orientation == VERTICAL){
        if (y + entityConstraints > HEIGHT){
            return attemptPlacementResponse(false, "Entity height exceeds grid dimensions");
        }

        for (int i = y; i <= y + entityConstraints - 1; i++){
            GridNodes potentialNode = battleshipGameGrid[i][x];
            if (potentialNode != EMPTY){
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
            if (potentialNode != EMPTY){
                return attemptPlacementResponse(false, "Can not place node in non-empty tile");
            }
        }

        for (int i = x; i <= x + entityConstraints - 1; i++){
            battleshipGameGrid[y][i] = node;
        }
    }

    return attemptPlacementResponse(true);
}

GameGrid::GameGrid() = default;
