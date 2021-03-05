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
    srand( time( nullptr ) );

    GameFlowController gameFlowController;

    Player hostPlayer("Suraj", gameFlowController);
    Player secondaryPlayer("Computer", gameFlowController);

    secondaryPlayer.setOpposingPlayer(&hostPlayer);
    hostPlayer.setOpposingPlayer(&secondaryPlayer);

    HostController hostController(&hostPlayer, &secondaryPlayer);

//    hostPlayer.deployShip(1, "A", 10, VERTICAL);
    hostPlayer.deployMine(9,0);
    secondaryPlayer.executeWarheadStrike("J", 1);

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
