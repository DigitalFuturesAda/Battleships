#include <iostream>
#include "tabulate.hpp"

#include "components/grid/GameGrid.h"
#include "components/player/Player.h"

int main() {
    GameGrid battleshipGameGrid;
    Player humanPlayer;

    battleshipGameGrid.attemptPlacement(2, 4, MINE, VERTICAL);
    battleshipGameGrid.attemptPlacement(2, 5, MINE, VERTICAL);

    battleshipGameGrid.attemptPlacement(2, 7, DESTROYED, VERTICAL);
    battleshipGameGrid.attemptPlacement(1, 7, DESTROYED, VERTICAL);

    battleshipGameGrid.attemptPlacement(5, 5, DESTROYER, VERTICAL);
    battleshipGameGrid.attemptPlacement(6, 5, CARRIER, VERTICAL);
    battleshipGameGrid.attemptPlacement(3, 2, SUBMARINE, VERTICAL);

    tabulate::Table battleshipGameTable;

    battleshipGameTable.add_row({"Game board", "Hit board"});
    battleshipGameTable.add_row({battleshipGameGrid.getGrid(), battleshipGameGrid.getGrid()});

    // This currently needs to be called after calls to GameGrid#getGrid.
    int width = battleshipGameGrid.getObservableGridWidth();

    battleshipGameTable.column(0).format().width(50);
    battleshipGameTable.column(1).format().width(50);

    std::cout << battleshipGameTable << std::endl;

    // Logic for Player01 - print out the grid and show the available ships below.
     humanPlayer.outputShipData();
}
