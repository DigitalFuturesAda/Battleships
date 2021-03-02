#include <iostream>
#include "lib/tabulate.hpp"

#include "components/grid/GameGrid.h"
#include "components/player/Player.h"

int main() {
    tabulate::Table battleshipGameTable;
    battleshipGameTable.add_row({"Game board", "Hit board"});

    Player humanPlayer;
    Player computerPlayer;

    humanPlayer.setOpposingPlayer(computerPlayer);
    computerPlayer.setOpposingPlayer(humanPlayer); // TODO(slyo): Potentially refactor into a PlayerContainer class.

    humanPlayer.fireWarheadStrikeAtOpposingPlayer("C", 5);

    // Any logic called after this will not be displayed in the board.
    battleshipGameTable.add_row({humanPlayer.battleshipGameGrid.getGrid(), humanPlayer.battleshipHitGrid.getGrid()});

    battleshipGameTable.add_row({humanPlayer.getStationaryShips(), humanPlayer.getDeployedShips()});

    battleshipGameTable.column(0).format().width(humanPlayer.battleshipGameGrid.getObservableGridWidth());
    battleshipGameTable.column(1).format().width(humanPlayer.battleshipHitGrid.getObservableGridWidth());

    std::cout << battleshipGameTable << std::endl;
}
