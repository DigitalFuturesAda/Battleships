#include <iostream>
#include "lib/tabulate.hpp"

#include "components/grid/GameGrid.h"
#include "components/player/Player.h"

int main() {
//    GameGrid battleshipGameGrid;
    Player humanPlayer;

    humanPlayer.battleshipGameGrid.attemptPlacement(2, 4, MINE, VERTICAL);
    humanPlayer.battleshipGameGrid.attemptPlacement(2, 5, MINE, VERTICAL);

    humanPlayer.battleshipGameGrid.attemptPlacement(2, 7, DESTROYED, VERTICAL);
    humanPlayer.battleshipGameGrid.attemptPlacement(1, 7, DESTROYED, VERTICAL);

    humanPlayer.battleshipGameGrid.attemptPlacement(5, 5, DESTROYER, VERTICAL);
    humanPlayer.battleshipGameGrid.attemptPlacement(6, 5, CARRIER, VERTICAL);
    humanPlayer.battleshipGameGrid.attemptPlacement(3, 2, SUBMARINE, VERTICAL);

    tabulate::Table battleshipGameTable;

    battleshipGameTable.add_row({"Game board", "Hit board"});
    battleshipGameTable.add_row({humanPlayer.battleshipGameGrid.getGrid(), humanPlayer.battleshipHitGrid.getGrid()});

    battleshipGameTable.add_row({humanPlayer.getStationaryShips(), humanPlayer.getDeployedShips()}).format().border_color(tabulate::Color::white);

    battleshipGameTable.column(0).format().width(humanPlayer.battleshipGameGrid.getObservableGridWidth());
    battleshipGameTable.column(1).format().width(humanPlayer.battleshipHitGrid.getObservableGridWidth());

    std::cout << battleshipGameTable << std::endl;
}
