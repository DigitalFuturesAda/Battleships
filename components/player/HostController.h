//
// Created by Suraj Lyons on 04/03/2021.
//

#ifndef BATTLESHIPS_HOSTCONTROLLER_H
#define BATTLESHIPS_HOSTCONTROLLER_H


#include "Player.h"

class HostController {
    Player *playerOneHost{};
    Player *playerTwo{};

public:
    HostController(Player *playerOneHost, Player *playerTwo) : playerOneHost(playerOneHost), playerTwo(playerTwo) {}

    void renderWinConditionInterface();

    bool hasEitherPlayerLost();
};


#endif //BATTLESHIPS_HOSTCONTROLLER_H
