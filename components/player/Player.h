//
// Created by Suraj Lyons on 01/03/2021.
//

#ifndef BATTLESHIPS_PLAYER_H
#define BATTLESHIPS_PLAYER_H

#include <utility>
#include <vector>
#include <map>
#include <regex>
#include "../ship/Ship.h"
#include "../grid/HitGrid.h"
#include "../../lib/tabulate.hpp"
#include "GameFlowController.h"

struct adjacentNodeEntry {
    std::string letter{};
    int yCoordinate{};

    adjacentNodeEntry(std::string letter, int yCoordinate) : letter(std::move(letter)), yCoordinate(yCoordinate) {}
    adjacentNodeEntry() = default;
};

class Player {
public:

    static const int MAX_WARHEAD_STRIKES_ATTEMPTS = 10000;
    static const int MAX_SHIP_DEPLOYMENT_ATTEMPTS = 10000;
    
    explicit Player(std::string playerName, GameFlowController& gameFlowController);

    void setOpposingPlayer(Player *player);
    attemptPlacementResponse deployShip(int shipVertexPosition, const std::string& letterIndex, int y, Orientation orientation);

    // Ship data utilities
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

    void renderCachedComputerWarheadDeploymentResponse(const attemptHitResponse& cachedHitResponse, bool isAutomaticAndRepeatedWarheadStrike = false, int repeatedWarheadStrikeAttempt = 0) const;

    void setPlayingAgainstComputer();

    bool deployWarshipAutomatically(int shipVertexPosition, int attempts = 0);

    attemptHitResponse deployWarheadStrikeAutomatically(int attempts = 0, bool isAutomaticAndRepeatedWarheadStrike = false);

    void deployWarheadStrikesAutomatically();

    void renderWarheadStrikeInterface();

    void deployWarshipsAutomatically();

    bool hasPlayerLostAllShips();

    std::vector<Ship> *getPlayerShips();

    void handleMineDetonationLogic(const attemptHitResponse& response);

    attemptPlacementResponse deployMine(int x, int y);

    void deployMultipleRandomlyPositionedMines();

    void renderSalvoWarheadStrikeInterface(bool isRepeatingInputSequence = false, int shipsThatHaveFiredValidWarheadStrikes = 0);

    std::string playerName = "Player";

    bool isComputer = false;

    std::map<std::string, bool> registeredMineDetonationAtRegion = {};
private:
    Player *opposingPlayer{};

    GameFlowController *gameFlowController;

    GameGrid battleshipGameGrid;
    HitGrid battleshipHitGrid;

    std::vector<Ship> playerShips{};

    bool deployShipInterface(int shipVertexPosition);

    bool alsoRenderComputerBoard = false;

    int getNumberOfOperationalShips();
};

#endif //BATTLESHIPS_PLAYER_H
