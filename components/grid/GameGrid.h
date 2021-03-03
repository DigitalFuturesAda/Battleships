#include <__bit_reference>
//
// Created by Suraj Lyons on 01/03/2021.
//

#include <iostream>
#include <utility>

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

struct attemptPlacementResponse {
    bool success = false;
    std::string message;
    attemptPlacementNodeHitResponse existingNode;

    explicit attemptPlacementResponse(
            bool success,
            std::string message = "") : success(success), message(std::move(message)) {};

    explicit attemptPlacementResponse(
            bool success,
            attemptPlacementNodeHitResponse existingNode): success(success), existingNode(existingNode) {}

    attemptPlacementResponse() = default;
};

struct attemptHitResponse {
    bool validAttempt;
    bool didHitTarget{};
    attemptPlacementNodeHitResponse hitNode;

    std::string message;

    attemptHitResponse(bool validAttempt, bool didHitTarget, attemptPlacementNodeHitResponse hitNode, std::string message = "") : validAttempt(validAttempt),
                                                                                  didHitTarget(didHitTarget),
                                                                                  hitNode(hitNode) {};

    attemptHitResponse(bool validAttempt, std::string message): validAttempt(validAttempt), message(std::move(message)) {};
};

class GameGrid {

private:
    static const int HEIGHT = 10;
    static const int WIDTH = 10;

    GridNodes battleshipGameGrid[HEIGHT][WIDTH] = { [0 ... HEIGHT - 1] = { [0 ... WIDTH - 1] = EMPTY } };

    static std::string formatNode(GridNodes node);

public:
    GameGrid();

    std::string renderGrid();
    static int getObservableGridWidth();
    static int getEntityConstraints(GridNodes placeableNodes);

    attemptPlacementResponse attemptPlacement(int x, int y, GridNodes node, Orientation orientation);
    attemptPlacementResponse attemptPlacement(std::string letter, int number, GridNodes node, Orientation orientation);
    attemptPlacementResponse checkIfNodeExists(std::string letter, int number);

    attemptHitResponse receiveWarheadStrike(std::string letter, int number);
};


#endif //BATTLESHIPS_GAMEGRID_H
