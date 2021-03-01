//
// Created by Suraj Lyons on 01/03/2021.
//

#include "Ship.h"

std::string Ship::getName() {
    switch (type){
        case EMPTY:
        case DESTROYED:
        case MINE:
            throw std::invalid_argument("Invalid ship type.");
        case CARRIER:
            return "Carrier";
        case BATTLESHIP:
            return "Battleship";
        case DESTROYER:
            return "Destroyer";
        case SUBMARINE:
            return "Submarine";
        case PATROL:
            return "Patrol Boat";
    }
}

int Ship::getXCoordinate() const {
    return x;
}

int Ship::getYCoordinate() const {
    return y;
}

bool Ship::isDeployed() const {
    return deployed;
}

Ship::Ship(GridNodes type, bool deployed) : type(type), deployed(deployed) {}
