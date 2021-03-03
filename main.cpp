#include <iostream>
#include "lib/tabulate.hpp"

#include "components/grid/GameGrid.h"
#include "components/player/Player.h"

int main() {
    Player humanPlayer("Suraj");
    Player computerPlayer("Computer");

    humanPlayer.setOpposingPlayer(&computerPlayer);
    computerPlayer.setOpposingPlayer(&humanPlayer);

    computerPlayer.deployWarshipsAutomatically();

    computerPlayer.renderPlayerUserInterface();

//    humanPlayer.renderPlayerUserInterface();
//    humanPlayer.showShipDeploymentInterface();
}
