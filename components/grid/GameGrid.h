//
// Created by Suraj Lyons on 01/03/2021.
//

#include <iostream>

#ifndef BATTLESHIPS_GAMEGRID_H
#define BATTLESHIPS_GAMEGRID_H

enum GridNodes {
    EMPTY = -1,
    DESTROYED = 0,
    MINE = 1,

    CARRIER,
    BATTLESHIP,
    DESTROYER,
    SUBMARINE,
    PATROL
};

enum Orientation {
    HORIZONTAL,
    VERTICAL
};

class GameGrid {

private:

    static const int HEIGHT = 10;
    static const int WIDTH = 10;

    GridNodes battleshipGameGrid[HEIGHT][WIDTH] = { [0 ... HEIGHT - 1] = { [0 ... WIDTH - 1] = EMPTY } };

    static std::string formatNode(GridNodes node);

    static int getEntityConstraints(GridNodes placeableNodes);

public:
    GameGrid();

    void printGrid();

    bool attemptPlacement(int x, int y, GridNodes node, Orientation orientation);
};


#endif //BATTLESHIPS_GAMEGRID_H