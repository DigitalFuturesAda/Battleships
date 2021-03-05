//
// Created by Suraj Lyons on 01/03/2021.
//

#include "Ship.h"

#include <utility>

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
        case CARRIER_MINE:
            return "Carrier";
        case BATTLESHIP:
        case BATTLESHIP_MINE:
            return "Battleship";
        case DESTROYER:
        case DESTROYER_MINE:
            return "Destroyer";
        case SUBMARINE:
        case SUBMARINE_MINE:
            return "Submarine";
        case PATROL:
        case PATROL_MINE:
            return "Patrol Boat";
    }
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

Ship Ship::setShipCoordinatePositions(std::vector<shipCoordinatePosition> coordinatePositions) {
    shipCoordinatePositions = std::move(coordinatePositions);

    return *this;
}

std::vector<shipCoordinatePosition> Ship::getShipCoordinatePositions() {
    return shipCoordinatePositions;
}

[[maybe_unused]] bool Ship::doesCoordinateIntersectShip(int x, int y) {
    for (auto&& coordinate: shipCoordinatePositions){
        if (coordinate.x == x && coordinate.y == y) return true;
    }

    return false;
}
