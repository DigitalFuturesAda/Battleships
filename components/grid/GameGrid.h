//
// Created by Suraj Lyons on 01/03/2021.
//

#include <iostream>
#include <utility>
#include <vector>

#ifndef BATTLESHIPS_GAMEGRID_H
#define BATTLESHIPS_GAMEGRID_H

enum GridNodes {
    // States
    EMPTY = -1,
    DESTROYED = 0,

    VALID_HIT,
    INVALID_HIT,

    // Misc objects
    MINE,

    // Ships
    CARRIER,
    BATTLESHIP,
    DESTROYER,
    SUBMARINE,
    PATROL,

    CARRIER_MINE,
    BATTLESHIP_MINE,
    DESTROYER_MINE,
    SUBMARINE_MINE,
    PATROL_MINE,

    UNKNOWN,
};

enum Orientation {
    HORIZONTAL,
    VERTICAL
};

struct attemptPlacementNodeHitResponse {
    int x{};
    int y{};

    GridNodes node = UNKNOWN;

    attemptPlacementNodeHitResponse() = default;
    explicit attemptPlacementNodeHitResponse(GridNodes node) : node(node) {}
    attemptPlacementNodeHitResponse(int x, int y, GridNodes node) : x(x), y(y), node(node) {}
};

struct shipCoordinatePosition {
    int x{};
    int y{};

    std::string letterIndex{};
    int gridYCoordinate{};

    explicit shipCoordinatePosition(int x, int y, std::string letterIndex, int gridYCoordinate) :
        x(x), y(y), letterIndex(std::move(letterIndex)), gridYCoordinate(gridYCoordinate) {}
    shipCoordinatePosition() = default;
};

struct attemptPlacementResponse {
    bool success = false;
    std::string message;
    attemptPlacementNodeHitResponse singleExistingNode;
    std::vector<shipCoordinatePosition> shipCoordinatePositions;

    /** Return a node placement response with an error message */
    explicit attemptPlacementResponse(
            bool success,
            std::string message = "") : success(success), message(std::move(message)) {};

    /** Return a node placement response with a single existingNode */
    explicit attemptPlacementResponse(
            bool success,
            attemptPlacementNodeHitResponse singleExistingNode): success(success), singleExistingNode(singleExistingNode) {};

    /** Return a node placement response with multiple (2 or more) ship coordinate positions */
    explicit attemptPlacementResponse(
            bool success,
            std::vector<shipCoordinatePosition> shipCoordinatePositions): success(success), shipCoordinatePositions(std::move(shipCoordinatePositions)) {};

    attemptPlacementResponse() = default;
};

struct attemptHitResponse {
    bool validAttempt = false;
    bool didHitTarget = false;
    attemptPlacementNodeHitResponse hitNode;

    std::string message;

    attemptHitResponse(bool validAttempt, bool didHitTarget, attemptPlacementNodeHitResponse hitNode, const std::string& message = "") : validAttempt(validAttempt),
                                                                                  didHitTarget(didHitTarget),
                                                                                  hitNode(hitNode) {};

    attemptHitResponse(bool validAttempt, std::string message): validAttempt(validAttempt), message(std::move(message)) {};

    attemptHitResponse() = default;
};

class GameGrid {
public:
    GameGrid();

    std::string renderGrid();
    int getObservableGridWidth();
    static int getEntityConstraints(GridNodes placeableNodes);

    attemptPlacementResponse attemptPlacement(int x, int y, GridNodes node, Orientation orientation);
    attemptPlacementResponse attemptPlacement(std::string letter, int number, GridNodes node, Orientation orientation);
    attemptPlacementResponse checkIfNodeExists(std::string letter, int number);

    attemptHitResponse receiveWarheadStrike(std::string letter, int number);

    std::vector<std::vector<GridNodes>> battleshipGameGrid = {};

    [[nodiscard]] int getGridHeight() const;

    [[nodiscard]] int getGridWidth() const;

private:
    static std::string formatNode(GridNodes node);

    int gridHeight;
    int gridWidth;
};


#endif //BATTLESHIPS_GAMEGRID_H
