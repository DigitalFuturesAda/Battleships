#include <iostream>
#include "lib/tabulate.hpp"

#include "components/grid/GameGrid.h"
#include "components/player/Player.h"
#include "components/util/io.h"
#include "components/player/HostController.h"
#include <thread>
#include <chrono>

int main() {
    srand( time( nullptr ) );

    Player humanPlayer("Suraj");
    Player computerPlayer("Computer");

    computerPlayer.setOpposingPlayer(&humanPlayer);
    humanPlayer.setOpposingPlayer(&computerPlayer);

    HostController hostController(&humanPlayer, &computerPlayer);

    computerPlayer.deployWarshipsAutomatically();
    humanPlayer.deployWarshipsAutomatically();
    
    // Render the ship deployment UI with a sample of the board, this will be no-op if all ships are deployed.
    humanPlayer.renderPlayerUserInterface();
    humanPlayer.showShipDeploymentInterface();

    // We call this afterwards, as this augments the computer board onto the player board
    humanPlayer.setPlayingAgainstComputer();

    // Show the interface having enabled the computer board. Any board mutations should happen before this call.
    humanPlayer.renderPlayerUserInterface();

    // Warhead strike interface
    while (true){
        if (hostController.hasEitherPlayerLost()){
            hostController.renderWinConditionInterface();
            break;
        } else {
            humanPlayer.renderWarheadStrikeInterface();
            computerPlayer.deployWarheadStrikeAutomatically();
            humanPlayer.renderPlayerUserInterface();
        };
    }
}
