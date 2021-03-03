//
// Created by Suraj Lyons on 01/03/2021.
//

#ifndef BATTLESHIPS_PLAYER_H
#define BATTLESHIPS_PLAYER_H

#include <utility>
#include <vector>
#include <map>
#include "../ship/Ship.h"
#include "../grid/HitGrid.h"
#include "../../lib/tabulate.hpp"

class Player {
public:
    explicit Player(std::string playerName);

    void setOpposingPlayer(Player *player);
    attemptPlacementResponse deployShip(GridNodes shipType, const std::string& x, int y, Orientation orientation);

    // Ship data utilities
    std::string getDeployedShips();
    std::string getStationaryShips();
    std::vector<std::string> getShipInformation();

    attemptHitResponse executeWarheadStrike(std::string letter, int number);

    GameGrid *getGameGrid();
    HitGrid *getHitGrid();

    void renderPlayerGrid(bool alsoRenderComputerBoard = false);
    void renderStatisticsBoard();

    void showShipDeploymentInterface();

    void renderPlayerUserInterface(bool alsoRenderComputerBoard = false);

private:
    Player *opposingPlayer{};

    GameGrid battleshipGameGrid;
    HitGrid battleshipHitGrid;

    std::vector<Ship> playerShips{};

    bool deployShipInterface(Ship ship);

    std::string playerName;
};


#endif //BATTLESHIPS_PLAYER_H
