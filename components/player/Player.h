//
// Created by Suraj Lyons on 01/03/2021.
//

#ifndef BATTLESHIPS_PLAYER_H
#define BATTLESHIPS_PLAYER_H

#include <vector>
#include "../ship/Ship.h"

class Player {
public:
    Player();

    std::vector<Ship> playerShips;
    GameGrid battleshipGameGrid;
    GameGrid battleshipHitGrid;

    std::string getDeployedShips();
    std::string getStationaryShips();
    std::string getShipData();

    bool attemptToDeployShip(Ship);
};


#endif //BATTLESHIPS_PLAYER_H
