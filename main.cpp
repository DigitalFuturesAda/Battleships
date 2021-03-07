#include <iostream>
#include "lib/tabulate.hpp"

#include "components/grid/GameGrid.h"
#include "components/player/Player.h"
#include "components/util/io.h"
#include "components/player/HostController.h"
#include "components/player/GameFlowController.h"
#include "components/util/rand.h"
#include <thread>
#include <chrono>

int main() {
    srand( time( nullptr ) );

    // TODO(slyo): Refactor this so that it inherits from a configuration class.
    constexpr bool IS_PLAYING_SALVO = false;

    GameFlowController gameFlowController;

    Player hostPlayer("Jarvis", gameFlowController);
    Player secondaryPlayer("Ultron", gameFlowController);

    secondaryPlayer.setOpposingPlayer(&hostPlayer);
    hostPlayer.setOpposingPlayer(&secondaryPlayer);

    HostController hostController(&hostPlayer, &secondaryPlayer);

    hostPlayer.deployWarshipsAutomatically();
    secondaryPlayer.deployWarshipsAutomatically();

    // Render the ship deployment UI with a sample of the board, this will be no-op if all ships are deployed.
    hostPlayer.showShipDeploymentInterface();

    // We call this afterwards, as this augments the computer board onto the player board
    hostPlayer.setPlayingAgainstComputer();
    secondaryPlayer.setPlayingAgainstComputer();

    // These calls should happen AFTER all ships have been deployed
    hostPlayer.deployMultipleRandomlyPositionedMines();
    secondaryPlayer.deployMultipleRandomlyPositionedMines();

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

            hostPlayer.deployWarheadStrikeAutomatically();
            secondaryPlayer.deployWarheadStrikeAutomatically();
        };
    }
}
