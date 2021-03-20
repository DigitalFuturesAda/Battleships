//
// Created by Suraj Lyons on 01/03/2021.
//

#include "Ship.h"
#include "../util/rand.h"

#include <utility>

Ship::Ship(GridNodes type, bool deployed) : type(type), deployed(deployed) {
    this->maxLives = GameGrid::getEntityConstraints(type);
    this->lives = maxLives;
    this->id = randomBetween19937(INT_MIN, INT_MAX);
}

Ship::Ship(GridNodes type) : type(type) {
    this->maxLives = GameGrid::getEntityConstraints(type);
    this->lives = maxLives;
    this->id = randomBetween19937(INT_MIN, INT_MAX);
}

GridNodes Ship::shipNameToGridNode(const std::string& shipName){
    if (shipName == "Carrier"){
        return CARRIER;
    } else if (shipName == "Battleship"){
        return BATTLESHIP;
    } else if (shipName == "Destroyer"){
        return DESTROYER;
    } else if (shipName == "Submarine"){
        return SUBMARINE;
    } else if (shipName == "Patrol Boat"){
        return PATROL;
    }

    throw std::runtime_error("Unrecognised ship - " + shipName);
}

std::string Ship::getShipName(GridNodes type) {
    switch (type) {
        case EMPTY:
        case DESTROYED:
        case VALID_HIT:
        case INVALID_HIT:
        case UNKNOWN:
            throw std::runtime_error("Ship#getName called with an invalid Invalid ship type");
        case MINE:
            return "Mine";
        // These MUST remain consistent between the ini configuration file
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

std::string Ship::getName() {
    return Ship::getShipName(type);
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
    if (lives < 1){
        lives = 0;
        return *this;
    }

    lives = lives_;
    return *this;
}

bool Ship::isSunk() const {
    return lives <= 0;
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

Ship Ship::setTakenHitFromCoordinate(const std::string& coordinateNotation) {

    this->takenHitsFromCoordinate.emplace_back(coordinateNotation);

    return *this;
}

bool Ship::hasTakenHitFromCoordinate(const std::string &coordinateNotation) {
    for (auto&& notation: takenHitsFromCoordinate){
        if (notation == coordinateNotation) return true;
    }
    return false;
}

int Ship::getId() {
    return id;
}
