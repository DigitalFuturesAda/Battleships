//
// Created by Suraj Lyons on 01/03/2021.
//

#include "GameGrid.h"
#include <iostream>
#include <absl/strings/str_format.h>

void GameGrid::printGrid() {
    int verticalCounter = 1;

    for (const auto& gridRow : battleshipGameGrid){
        std::cout << absl::StrFormat("%-2d", verticalCounter) << " ";
        for (const GridNodes node : gridRow){
            std::cout << formatNode(node) << "";
        }
        std::cout << std::endl;

        verticalCounter ++;
    }
}

std::string GameGrid::formatNode(GridNodes node) {
    switch (node) {
        case EMPTY:
            return "[ ]";
        case DESTROYED:
            return " X ";
        case MINE:
            return " M ";
        case CARRIER:
            return " C ";
        case BATTLESHIP:
            return " B ";
        case DESTROYER:
            return " D ";
        case SUBMARINE:
            return " S ";
        case PATROL:
            return " P ";
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

bool GameGrid::attemptPlacement(int x, int y, GridNodes node, Orientation orientation) {
    int entityConstraints = getEntityConstraints(node);

    // Ensure x,y coords are within the confines of the grid.
    if (y > HEIGHT || x > WIDTH){
        return false;
    }

    // Ensure we're not trying to place the starting node on a taken tile.
    GridNodes existingNode = battleshipGameGrid[y][x];
    if (existingNode != EMPTY){
        std::cout << "Can not place starting node in non-empty tile" << std::endl;
        return false;
    }

    if (orientation == VERTICAL){
        if (y + entityConstraints > HEIGHT){
            std::cout << "Entity height exceeds grid dimensions" << std::endl;
            return false;
        }

        for (int i = y; i <= y + entityConstraints - 1; i++){
            GridNodes potentialNode = battleshipGameGrid[i][x];
            if (potentialNode != EMPTY){
                std::cout << "Can not place node in non-empty tile" << std::endl;
                return false;
            }
        }

        for (int i = y; i <= y + entityConstraints - 1; i++){
            battleshipGameGrid[i][x] = node;
        }
    } else {
        if (x + entityConstraints > WIDTH){
            std::cout << "Entity width exceeds grid dimensions" << std::endl;
            return false;
        }

        for (int i = x; i <= x + entityConstraints - 1; i++){
            GridNodes potentialNode = battleshipGameGrid[y][i];
            if (potentialNode != EMPTY){
                std::cout << "Can not place node in non-empty tile" << std::endl;
                return false;
            }
        }

        for (int i = x; i <= x + entityConstraints - 1; i++){
            battleshipGameGrid[y][i] = node;
        }
    }

    return true;
}

GameGrid::GameGrid() = default;
