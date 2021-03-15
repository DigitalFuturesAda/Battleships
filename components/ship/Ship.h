//
// Created by Suraj Lyons on 01/03/2021.
//

#ifndef BATTLESHIPS_SHIP_H
#define BATTLESHIPS_SHIP_H


#include "../grid/GameGrid.h"

class Ship {

private:
    GridNodes type;
    Orientation orientation{};
    bool deployed = false;
    int lives = -1;
    int maxLives = -1;

public:
    Ship(GridNodes type, bool deployed);
    explicit Ship(GridNodes type);

    static std::string getShipName(GridNodes type);

    static GridNodes shipNameToGridNode(const std::string& shipName);

    std::string getName();
    [[nodiscard]] GridNodes getShipType() const;
    [[nodiscard]] int getLives() const;
    [[nodiscard]] int getMaxLives() const;

    [[nodiscard]] bool isDeployed() const;

    Ship setOrientation(Orientation orientation);

    Ship setDeployed();

    Ship setLives(int lives);

    [[nodiscard]] std::string getShipStatusFormatted() const;

    [[nodiscard]] bool isSunk() const;

    Ship setShipCoordinatePositions(std::vector<shipCoordinatePosition> coordinatePositions);

    std::vector<shipCoordinatePosition> getShipCoordinatePositions();

    [[maybe_unused]] bool doesCoordinateIntersectShip(int x, int y);

    Ship setTakenHitFromCoordinate(const std::string& coordinateNotation);

    bool hasTakenHitFromCoordinate(const std::string& coordinateNotation);

private:
    std::vector<shipCoordinatePosition> shipCoordinatePositions;

    std::vector<std::string> takenHitsFromCoordinate = {};
};


#endif //BATTLESHIPS_SHIP_H
