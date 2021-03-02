//
// Created by Suraj Lyons on 01/03/2021.
//

#ifndef BATTLESHIPS_PLAYER_H
#define BATTLESHIPS_PLAYER_H

#include <vector>
#include <map>
#include "../ship/Ship.h"
#include "../grid/HitGrid.h"

class Player {
public:
    Player();

    void setOpposingPlayer(Player *player);
    attemptPlacementResponse attemptToDeployShip(GridNodes shipType, const std::string& x, int y, Orientation orientation);

    // Ship data utilities
    std::string getDeployedShips();
    std::string getStationaryShips();
    std::string getShipData();

    std::string name;

    attemptHitResponse fireWarheadStrikeAtOpposingPlayer(std::string letter, int number);

    GameGrid battleshipGameGrid;
    HitGrid battleshipHitGrid;

    GameGrid *getGameGrid();
    HitGrid *getHitGrid();

private:
    Player *opposingPlayer;

    std::vector<Ship> playerShips;
};


#endif //BATTLESHIPS_PLAYER_H
