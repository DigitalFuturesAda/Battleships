#include <iostream>
#include <tcl.h>

#include <locale>
#include <codecvt>
#include <string>

#include "components/grid/GameGrid.h"
#include "components/player/Player.h"

int main() {
    GameGrid battleshipGameGrid;
    Player humanPlayer;

//    battleshipGameGrid.attemptPlacement(2, 4, MINE, VERTICAL);
//    battleshipGameGrid.attemptPlacement(2, 5, MINE, VERTICAL);
//
//    battleshipGameGrid.attemptPlacement(2, 7, DESTROYED, VERTICAL);
//    battleshipGameGrid.attemptPlacement(1, 7, DESTROYED, VERTICAL);
//
//    battleshipGameGrid.attemptPlacement(5, 5, DESTROYER, VERTICAL);
//    battleshipGameGrid.attemptPlacement(6, 5, CARRIER, VERTICAL);
//    battleshipGameGrid.attemptPlacement(3, 2, SUBMARINE, VERTICAL);

    std::cout << battleshipGameGrid.getGrid();

    // Logic for Player01 - print out the grid and show the available ships below.

    // battleshipGameGrid.outputGrid();
    // humanPlayer.outputShipData();

}
