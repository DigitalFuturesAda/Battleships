#include <iostream>
#include "lib/tabulate.hpp"

#include "components/grid/GameGrid.h"
#include "components/player/Player.h"
#include "components/util/io.h"
#include "components/player/HostController.h"
#include "components/player/GameFlowController.h"
#include <thread>
#include <chrono>

int main() {
//    srand( time( nullptr ) );

    GameFlowController gameFlowController;

    Player hostPlayer("Suraj", gameFlowController);
    Player secondaryPlayer("Computer", gameFlowController);

    secondaryPlayer.setOpposingPlayer(&hostPlayer);
    hostPlayer.setOpposingPlayer(&secondaryPlayer);

    HostController hostController(&hostPlayer, &secondaryPlayer);

    hostPlayer.deployShip(0, "A", 5, HORIZONTAL);

    hostPlayer.deployWarshipsAutomatically();
    hostPlayer.deployMine(0, 0);
    hostPlayer.deployMine(1, 1);
    hostPlayer.deployMine(2, 2);
    hostPlayer.deployMine(3, 3);
    hostPlayer.deployMine(4, 4);
    hostPlayer.deployMine(5, 5);
    hostPlayer.deployMine(6, 6);
    hostPlayer.deployMine(7, 7);
    hostPlayer.deployMine(8, 8);
    hostPlayer.deployMine(9, 9);

    hostPlayer.deployMine(9, 0);
    hostPlayer.deployMine(8, 1);
    hostPlayer.deployMine(7, 2);
    hostPlayer.deployMine(6, 3);
    hostPlayer.deployMine(5, 4);
    hostPlayer.deployMine(4, 5);
    hostPlayer.deployMine(3, 6);
    hostPlayer.deployMine(2, 7);
    hostPlayer.deployMine(1, 8);
    hostPlayer.deployMine(0, 9);
    secondaryPlayer.executeWarheadStrike("A", 0);


//    hostPlayer.deployMine(0, 3);
//    hostPlayer.deployMine(2, 4);
//    secondaryPlayer.executeWarheadStrike("A", 4);
//    secondaryPlayer.executeWarheadStrike("C", 5);

    hostPlayer.renderPlayerUserInterface();

    /*
    hostPlayer.deployWarshipsAutomatically();
    secondaryPlayer.deployWarshipsAutomatically();

    // Render the ship deployment UI with a sample of the board, this will be no-op if all ships are deployed.
     hostPlayer.renderPlayerUserInterface();
     hostPlayer.showShipDeploymentInterface();

    // We call this afterwards, as this augments the computer board onto the player board
    hostPlayer.setPlayingAgainstComputer();

    while (true){
        if (gameFlowController.hasUserRequestedToRestart()){
            clearConsole();
            displayInformation("Resetting board. Stand by...\n", 1);
            break;
        }

        if (hostController.hasEitherPlayerLost()){
            hostController.renderWinConditionInterface();
            break;
        } else {
            hostPlayer.renderPlayerUserInterface();
            hostPlayer.renderWarheadStrikeInterface();

            secondaryPlayer.deployWarheadStrikeAutomatically();
        };
    }
    */
}
