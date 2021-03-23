//
// Created by Suraj Lyons on 01/03/2021.
//

#ifndef BATTLESHIPS_PLAYER_H
#define BATTLESHIPS_PLAYER_H

#include <utility>
#include <vector>
#include <map>
#include <regex>
#include <queue>
#include <deque>
#include "../ship/Ship.h"
#include "../grid/HitGrid.h"
#include "../../lib/tabulate.hpp"
#include "GameFlowController.h"
#include "../menu/MenuHelper.h"

struct adjacentNodeEntry {
    std::string letter{};
    int yCoordinate{};

    adjacentNodeEntry(std::string letter, int yCoordinate) : letter(std::move(letter)), yCoordinate(yCoordinate) {}
    adjacentNodeEntry() = default;
};

struct nodeEntryCoordinate {
    int x {};
    int y {};

    nodeEntryCoordinate(int x, int y) : x(x), y(y) {}

    nodeEntryCoordinate() = default;
};

enum HuntMode {
    HUNT,
    DESTROY
};

enum HitNode {
    HIT_NODE_MINE,
    HIT_NODE_SHIP,
    HIT_NODE_UNKNOWN
};

enum ShipHuntCoordinateMetadataNodePosition {
    ABOVE = 1,
    BELOW = 2,
    LEFT = 3,
    RIGHT= 4,

    PRE = 5,
    POST = 6,

    NODE_POSITION_UNKNOWN = -1
};

struct ShipHuntCoordinateMetadata {
    ShipHuntCoordinateMetadataNodePosition nodePosition = NODE_POSITION_UNKNOWN;
    adjacentNodeEntry nodeEntry;

    ShipHuntCoordinateMetadata(ShipHuntCoordinateMetadataNodePosition nodePosition, adjacentNodeEntry nodeEntry)
            : nodePosition(nodePosition), nodeEntry(std::move(nodeEntry)) {}
};

enum ShipHuntOrientation {
    SHIP_HUNT_VERTICAL,
    SHIP_HUNT_HORIZONTAL,

    SHIP_HUNT_ORIENTATION_UNKNOWN
};

struct ShipHunt {
    std::vector<shipCoordinatePosition> shipCoordinates;
    std::deque<ShipHuntCoordinateMetadata> potentialCoordinatesQueue;

    ShipHuntOrientation shipOrientation = SHIP_HUNT_ORIENTATION_UNKNOWN;

    std::vector<std::string> ignoredCoordinates;

    int maxHealth = 0;
    int currentHealth = 0;

    ShipHunt() = default;

    ShipHunt(std::vector<shipCoordinatePosition> shipCoordinates,
             std::deque<ShipHuntCoordinateMetadata> potentialCoordinatesQueue) : shipCoordinates(std::move(shipCoordinates)),
                                                                        potentialCoordinatesQueue(std::move(potentialCoordinatesQueue)) {}
};

class Player {
public:

    static const int MAX_WARHEAD_STRIKES_ATTEMPTS = 10000;
    static const int MAX_SHIP_DEPLOYMENT_ATTEMPTS = 10000;

    /**
     * Controls whether the confirmation dialog ("Press enter to continue") should be shown when it's only computer
     * versus computer.
     */
    static const bool SHOULD_SHOW_CONTINUE_GAME_CONFIRMATION_DIALOG_DURING_AUTOMATION = false;

    /**
     * Controls whether any log statements should be output.
     */
    static const bool SHOULD_SHOW_LOG_STATEMENTS_DURING_AUTOMATION = true;
    
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

    void renderCachedComputerWarheadDeploymentResponse(const attemptHitResponse& cachedHitResponse, bool isAutomaticAndRepeatedWarheadStrike = false, int repeatedWarheadStrikeAttempt = 0);

    bool deployWarshipAutomatically(int shipVertexPosition, int attempts = 0);

    attemptHitResponse deployWarheadStrikeAutomatically(int attempts = 0, bool isAutomaticAndRepeatedWarheadStrike = false);

    attemptHitResponse executeAutomaticEnhancedAlgorithmWarheadStrike();

    attemptHitResponse executeHuntModeWarheadStrikeEnhancedAlgorithm(const std::string& letter, int y);

    attemptHitResponse executeDestroyModeWarheadStrikeEnhancedAlgorithm();

    void deployWarheadStrikesAutomatically();

    void renderWarheadStrikeInterface();

    void deployWarshipsAutomatically();

    bool hasPlayerLostAllShips();

    std::vector<Ship> *getPlayerShips();

    void handleMineDetonationLogic(const attemptHitResponse& response);

    attemptPlacementResponse deployMine(int x, int y);

    void deployMultipleRandomlyPositionedMines();

    Ship getIntersectingShip(int x, int y);

    void renderSalvoWarheadStrikeInterface(bool isRepeatingInputSequence = false, int shipsThatHaveFiredValidWarheadStrikes = 0);

    std::string playerName = "Player";

    bool isComputer = false;

    std::map<std::string, bool> registeredMineDetonationAtRegion = {};

    bool isComputerPlayingAgainstComputer();

    bool shouldShowContinueGameConfirmationDialog();

    int numberOfAttempts = 0;

    bool hasDeployedShip = false;

    bool shouldRenderLogStatements();

    void setPlayingAgainstComputer();

    void setPlayerType(PlayerType type);

    PlayerType getPlayerType();

private:
    Player *opposingPlayer{};

    PlayerType playerType;

    GameFlowController *gameFlowController;

    GameGrid battleshipGameGrid;
    HitGrid battleshipHitGrid;

    std::vector<Ship> playerShips = {};

    bool deployShipInterface(int shipVertexPosition);

    bool alsoRenderComputerBoard = false;

    int getNumberOfOperationalShips();

    std::vector<nodeEntryCoordinate> potentialNodes;

    void pushNodeAsAlphaIntoVector(int x, int y, std::vector<adjacentNodeEntry> *adjacentNodeEntries);

    void pushNodeAsAlphaIntoQueue(int x, int y, ShipHuntCoordinateMetadataNodePosition position, std::deque<ShipHuntCoordinateMetadata> *adjacentNodeEntries);

    bool isOutOfBounds(int x, int y);

    void initiatePlayerShipsVector();

    std::vector<adjacentNodeEntry> getAdjacentNodes(int x, int y);

    HuntMode targetMode = HUNT;

    std::map<int, ShipHunt> shipHuntCache = {};

    std::queue<int> shipHuntIdQueue = {};
};

#endif //BATTLESHIPS_PLAYER_H
