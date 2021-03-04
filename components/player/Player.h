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

    static const int MAX_WARHEAD_STRIKES_ATTEMPTS = 10000;
    static const int MAX_SHIP_DEPLOYMENT_ATTEMPTS = 10000;

    explicit Player(std::string playerName);

    void setOpposingPlayer(Player *player);
    attemptPlacementResponse deployShip(int shipVertexPosition, const std::string& letterIndex, int y, Orientation orientation);

    // Ship data utilities
    std::string getDeployedShips();
    std::string getStationaryShips();
    std::vector<std::string> getShipInformation();

    attemptHitResponse executeWarheadStrike(std::string letter, int number);

    GameGrid *getGameGrid();
    HitGrid *getHitGrid();

    void renderPlayerGrid();
    void renderStatisticsBoard();

    void showShipDeploymentInterface();

    void renderPlayerUserInterface();

    void setPlayingAgainstComputer();

    bool deployWarshipAutomatically(int shipVertexPosition, int attempts = 0);

    attemptHitResponse deployWarheadStrikeAutomatically(int attempts = 0);

    void renderWarheadStrikeInterface();

    void deployWarshipsAutomatically();

    bool hasPlayerLostAllShips();

    std::vector<Ship> *getPlayerShips();

    std::string playerName = "Player";
private:
    Player *opposingPlayer{};

    GameGrid battleshipGameGrid;
    HitGrid battleshipHitGrid;

    std::vector<Ship> playerShips{};

    bool deployShipInterface(int shipVertexPosition);

    bool alsoRenderComputerBoard = false;
};


#endif //BATTLESHIPS_PLAYER_H
