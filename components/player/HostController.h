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
    HostController(Player *playerOneHost, Player *playerTwo) : playerOneHost(playerOneHost), playerTwo(playerTwo) {
        playerOneHost->setOpposingPlayer(playerTwo);
        playerTwo->setOpposingPlayer(playerOneHost);
    }

    void renderWinConditionInterface();

    bool hasEitherPlayerLost();

    void switchCurrentPlayer();

    Player &getPlayer();

private:
    bool isPlayerOneTurnBool = true;
};


#endif //BATTLESHIPS_HOSTCONTROLLER_H
