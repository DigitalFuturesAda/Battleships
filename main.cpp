#include <iostream>
#include "lib/tabulate.hpp"

#include "components/grid/GameGrid.h"
#include "components/player/Player.h"
#include "components/util/io.h"
#include "components/player/HostController.h"
#include "components/player/GameFlowController.h"
#include "components/config/ConfigValidator.h"
#include "components/config/ConfigSingleton.h"
#include "components/menu/MenuHelper.h"
#include <thread>
#include <chrono>

int main() {
    srand( time( nullptr ) );

    GameFlowController gameFlowController;
    MenuHelper menuHelper;

    ConfigValidator configValidator("config.ini");
    ConfigSingleton::getInstance()->setValidator(configValidator);

    menuHelper.renderMenu();
    MenuGameConfiguration gameConfiguration = menuHelper.getGameConfiguration();

    if (gameConfiguration.requestToQuit){
        displayInformation("Exiting!", 0);
        exit(EXIT_SUCCESS);
    }

    Player hostPlayer(gameConfiguration.playerOneType == PLAYER ? "Player1" : "Computer1", gameFlowController);
    Player secondaryPlayer(gameConfiguration.playerTwoType == PLAYER ? "Player2" : "Computer2", gameFlowController);

    hostPlayer.setPlayerType(gameConfiguration.playerOneType);
    secondaryPlayer.setPlayerType(gameConfiguration.playerTwoType);

    HostController hostController(&hostPlayer, &secondaryPlayer);

    for (Player* player : {&hostPlayer, &secondaryPlayer}){
        if (player->isComputer)
            player->deployWarshipsAutomatically();
        else
            player->showShipDeploymentInterface();
    }

    // We call this afterwards, as this augments the computer board onto the player board
    if (gameConfiguration.playerOneType == COMPUTER) secondaryPlayer.setPlayingAgainstComputer();
    if (gameConfiguration.playerTwoType == COMPUTER) hostPlayer.setPlayingAgainstComputer();

    // These calls should happen AFTER all ships have been deployed
    if (gameConfiguration.hiddenMinesGameMode){
        hostPlayer.deployMultipleRandomlyPositionedMines();
        secondaryPlayer.deployMultipleRandomlyPositionedMines();
    }

    while (true){
        if (gameFlowController.hasUserRequestedToRestart() || hostController.hasEitherPlayerLost()) break;

        Player *currentPlayer = &hostController.getPlayer();

        if (currentPlayer->getPlayerType() == PLAYER){
            currentPlayer->renderPlayerUserInterface();
            if (gameConfiguration.salvoGameMode){
                currentPlayer->renderSalvoWarheadStrikeInterface();
            } else {
                currentPlayer->renderWarheadStrikeInterface();
            }
        } else {
            if (gameConfiguration.salvoGameMode){
                currentPlayer->deployWarheadStrikesAutomatically();
            } else {
                currentPlayer->deployWarheadStrikeAutomatically();
            }
        }

        hostController.switchCurrentPlayer();
    };

    if (gameFlowController.hasUserRequestedToRestart()){
        clearConsole();
        displayInformation("Resetting board. Stand by...\n", 1);
        return main();
    }

    if (hostController.hasEitherPlayerLost()){
        hostController.renderWinConditionInterface();
    };

    return EXIT_SUCCESS;
}

int debugMode() {
    srand( time( nullptr ) );

    GameFlowController gameFlowController;

    ConfigValidator configValidator("config.ini");
    ConfigSingleton::getInstance()->setValidator(configValidator);

    Player hostPlayer("Suraj", gameFlowController);
    Player secondaryPlayer("Computer", gameFlowController);
    HostController hostController(&hostPlayer, &secondaryPlayer);

    hostPlayer.setPlayerType(COMPUTER);
    secondaryPlayer.setPlayerType(COMPUTER);

    hostPlayer.deployWarshipsAutomatically();
    secondaryPlayer.deployWarshipsAutomatically();

    hostPlayer.setPlayingAgainstComputer();
    secondaryPlayer.setPlayingAgainstComputer();

    while (true){
        if (gameFlowController.hasUserRequestedToRestart()){
            clearConsole();
            displayInformation("Resetting board. Stand by...\n", 1);
            break;
        };

        if (hostController.hasEitherPlayerLost()){
            hostController.renderWinConditionInterface();
            break;
        } else {
            hostPlayer.deployWarheadStrikeAutomatically();
            secondaryPlayer.deployWarheadStrikeAutomatically();
        };
    }

    return EXIT_SUCCESS;
};