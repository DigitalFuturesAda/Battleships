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

int main2() {
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

    if (gameConfiguration.playerOneType == COMPUTER){
        hostPlayer.deployWarshipsAutomatically();
    } else {
        // Render the ship deployment UI with a sample of the board, this will be no-op if all ships are deployed.
        hostPlayer.showShipDeploymentInterface();
    }

    if (gameConfiguration.playerTwoType == COMPUTER){
        secondaryPlayer.deployWarshipsAutomatically();
    } else {
        secondaryPlayer.showShipDeploymentInterface();
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
        if (gameFlowController.hasUserRequestedToRestart()){
            clearConsole();
            displayInformation("Resetting board. Stand by...\n", 1);
            break;
        };

        if (hostController.hasEitherPlayerLost()){
            hostController.renderWinConditionInterface();
            break;
        } else {
            Player *currentPlayer = hostController.isPlayerOneTurn() ? &hostPlayer : &secondaryPlayer;

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
    }
}

int main() {
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
//            Player *currentPlayer = hostController.isPlayerOneTurn() ? &hostPlayer : &secondaryPlayer;
            hostPlayer.deployWarheadStrikeAutomatically();
            secondaryPlayer.deployWarheadStrikeAutomatically();
//            hostController.switchCurrentPlayer();
        };
    }
};