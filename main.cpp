#include <iostream>
#include "lib/tabulate.hpp"

#include "components/grid/GameGrid.h"
#include "components/player/Player.h"

int main() {
    // Testing logic
    Player humanPlayer("Suraj");
    Player computerPlayer("Computer");

    humanPlayer.setOpposingPlayer(&computerPlayer);
    computerPlayer.setOpposingPlayer(&humanPlayer);

    humanPlayer.deployShip(CARRIER, "A", 1, VERTICAL);
    humanPlayer.deployShip(SUBMARINE, "C", 1, VERTICAL);

    humanPlayer.renderPlayerGrid();
    humanPlayer.renderStatisticsBoard();
}
