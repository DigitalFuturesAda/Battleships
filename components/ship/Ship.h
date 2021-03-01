//
// Created by Suraj Lyons on 01/03/2021.
//

#ifndef BATTLESHIPS_SHIP_H
#define BATTLESHIPS_SHIP_H


#include "../grid/GameGrid.h"

class Ship {

private:
    GridNodes type;
    bool deployed;

    int x = -1;
    int y = -1;

public:
    Ship(GridNodes type, bool deployed);

    std::string getName();

    int getXCoordinate() const;
    int getYCoordinate() const;

    bool isDeployed() const;
};


#endif //BATTLESHIPS_SHIP_H
