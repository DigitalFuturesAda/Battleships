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

    void outputDeployedShips();
    void outputStationaryShips();
    void outputShipData();

    bool attemptToDeployShip(Ship);
};


#endif //BATTLESHIPS_PLAYER_H
