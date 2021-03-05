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
#include "GameFlowController.h"

class Player {
public:

    static const int MAX_WARHEAD_STRIKES_ATTEMPTS = 10000;
    static const int MAX_SHIP_DEPLOYMENT_ATTEMPTS = 10000;

    explicit Player(std::string playerName, GameFlowController& gameFlowController);

    void setOpposingPlayer(Player *player);
    attemptPlacementResponse deployShip(int shipVertexPosition, const std::string& letterIndex, int y, Orientation orientation);

    // Ship data utilities
    std::string getDeployedShips();
    std::string getStationaryShips();
    std::vector<std::string> getShipInformation();

    attemptHitResponse executeWarheadStrike(std::string letter, int number);

    GameGrid *getGameGrid();
    HitGrid *getHitGrid();

    [[nodiscard]] GameFlowController * getGameFlowController() const;

    void renderPlayerGrid();
    void renderStatisticsBoard();

    void showShipDeploymentInterface();

    void renderPlayerUserInterface();

    tabulate::Table getPlayerShipStatisticsBoard();

    void renderCachedComputerWarheadDeploymentResponse(const attemptHitResponse& cachedHitResponse) const;

    void setPlayingAgainstComputer();

    bool deployWarshipAutomatically(int shipVertexPosition, int attempts = 0);

    attemptHitResponse deployWarheadStrikeAutomatically(int attempts = 0);

    void renderWarheadStrikeInterface();

    void deployWarshipsAutomatically();

    bool hasPlayerLostAllShips();

    std::vector<Ship> *getPlayerShips();

    std::string playerName = "Player";

    bool isComputer = false;
private:
    Player *opposingPlayer{};

    GameFlowController *gameFlowController;

    GameGrid battleshipGameGrid;
    HitGrid battleshipHitGrid;

    std::vector<Ship> playerShips{};

    bool deployShipInterface(int shipVertexPosition);

    bool alsoRenderComputerBoard = false;
};

#endif //BATTLESHIPS_PLAYER_H
