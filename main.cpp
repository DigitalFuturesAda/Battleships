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

    tabulate::Table styled_table;

    styled_table.add_row({"Game board", "Hit board"});
    styled_table.column(0).format().width(35);
    styled_table.column(1).format().width(35);
    styled_table.add_row({battleshipGameGrid.getGrid(), battleshipGameGrid.getGrid()});

    std::cout << styled_table << std::endl;

    // Logic for Player01 - print out the grid and show the available ships below.

    // battleshipGameGrid.outputGrid();
     humanPlayer.outputShipData();
}
