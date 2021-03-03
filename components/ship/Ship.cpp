//
// Created by Suraj Lyons on 01/03/2021.
//

#include "Ship.h"

Ship::Ship(GridNodes type, bool deployed) : type(type), deployed(deployed) {
    this->maxLives = GameGrid::getEntityConstraints(type);
    this->lives = maxLives;
}

Ship::Ship(GridNodes type) : type(type) {
    this->maxLives = GameGrid::getEntityConstraints(type);
    this->lives = maxLives;
}

std::string Ship::getName() {
    switch (type) {
        case EMPTY:
        case DESTROYED:
        case MINE:
        case VALID_HIT:
        case INVALID_HIT:
        case UNKNOWN:
            throw std::runtime_error("Ship#getName called with an invalid Invalid ship type");
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

    return "Test";
}

GridNodes Ship::getShipType() const {
    return type;
}

bool Ship::isDeployed() const {
    return deployed;
}

std::string Ship::getShipStatusFormatted() const {
    if (deployed) {
        if (isSunk()) {
            return "Sunk";
        }
        return "Deployed";
    } else {
        return "Stationary";
    }
}

Ship Ship::setOrientation(Orientation orientation_) {
    orientation = orientation_;

    return *this;
}

Ship Ship::setDeployed() {
    deployed = true;

    return *this;
}

int Ship::getLives() const {
    return lives;
}

int Ship::getMaxLives() const {
    return maxLives;
}

Ship Ship::setLives(int lives_) {
    lives = lives_;

    return *this;
}

bool Ship::isSunk() const {
    return lives == 0;
}
