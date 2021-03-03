#include <iostream>
#include "lib/tabulate.hpp"

#include "components/grid/GameGrid.h"
#include "components/player/Player.h"
#include "components/util/io.h"

int main() {
    Player humanPlayer("Suraj");
    Player computerPlayer("Computer");

    humanPlayer.setOpposingPlayer(&computerPlayer);
    computerPlayer.setOpposingPlayer(&humanPlayer);

    computerPlayer.deployWarshipsAutomatically();
    humanPlayer.deployWarshipsAutomatically();

    // Render the ship deployment UI with a sample of the board
    humanPlayer.renderPlayerUserInterface();
    humanPlayer.showShipDeploymentInterface();

    // We call this afterwards, as this augments the computer board onto the player board
    humanPlayer.setPlayingAgainstComputer();

    // Show the interface having enabled the computer board
    humanPlayer.renderPlayerUserInterface();

    // Warhead strike interface
    while (true){
        humanPlayer.renderWarheadStrikeInterface();
        humanPlayer.renderPlayerUserInterface();
    }

}
